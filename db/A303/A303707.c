/*
 * A303707  Number of factorizations of n using elements of A007916 (numbers
 *          that are not perfect powers).
 * https://oeis.org/A303707
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of factorizations of n using elements of A007916 *
 *            (numbers that are not perfect powers). The implementation uses a
 *            * recursive, memoised enumeration that searches over
 *            non‑perfect‑power * divisors in non‑decreasing order. It is
 *            suitable for n up to about 10 000 * and works with any...
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

typedef struct {
    uint64_t key;
    seq_t    val;
    unsigned used;
} HashEntry;

static bool   *allowed = NULL;      /* true ⇔ i is a non‑perfect‑power */
static size_t  allowed_size = 0;    /* current size of ‘allowed’ array   */
static HashEntry *hash_table = NULL;
static const size_t hash_size = (1 << 20);   /* 1 MiB, enough for the cache  */

/* ------------------------------------------------------------------ */
/* Initialise ‘allowed’ up to at least ‘limit’ (inclusive).  Numbers  */
/* that are perfect powers are marked false; all other >1 are true.   */
/* ------------------------------------------------------------------ */
static void init_allowed(idx_t limit)
{
    if (allowed && allowed_size >= limit)
        return;   /* already sufficient */

    bool *tmp = (bool *)realloc(allowed, (size_t)(limit + 1) * sizeof(bool));
    if (!tmp) abort();           /* memory error */
    allowed = tmp;

    /* Mark everything >1 as allowed initially */
    for (idx_t i = 2; i <= limit; ++i)
        allowed[i] = true;
    if (limit >= 0) allowed[0] = false;
    if (limit >= 1) allowed[1] = false;

    /* Mark perfect powers as not allowed */
    for (idx_t base = 2; base * base <= limit; ++base)
    {
        idx_t val = base * base;        /* base^2 */
        while (val <= limit)
        {
            allowed[val] = false;       /* perfect power */
            if (val > limit / base) break;   /* avoid overflow */
            val *= base;                /* next exponent */
        }
    }

    allowed_size = limit;
}

/* ------------------------------------------------------------------ */
/* Helper to compute 64‑bit hash position for key.  Power‑of‑two size  */
/* ------------------------------------------------------------------ */
static inline size_t hash_pos(uint64_t key)
{
    return key & (hash_size - 1);
}

/* ------------------------------------------------------------------ */
/* Memoised recursion: count factorizations of n with all factors ≥   */
/* min_factor.  The divisor list is generated in ascending order to  */
/* avoid duplicate factorizations.                                    */
/* ------------------------------------------------------------------ */
static seq_t count_factorizations(idx_t n, idx_t min_factor)
{
    /* Build key from the pair (n, min_factor).  Works for n ≤ 2^32. */
    uint64_t key = ((uint64_t)n << 32) | (uint64_t)min_factor;
    size_t pos = hash_pos(key);

    /* Linear probing to locate cached result or empty slot */
    while (hash_table[pos].used) {
        if (hash_table[pos].key == key)
            return hash_table[pos].val;   /* cached value */
        pos = hash_pos(pos + 1);
    }

    /* Compute value */
    seq_t result = 0;
    if (n >= min_factor && allowed[n])
        result = 1;   /* the trivial factorization {n} */

    /* Enumerate allowed divisors d ≥ min_factor, with d ≤ n/d to keep    */
    /* factors non‑decreasing (e.g., 2*2*3 is counted once, not 2*3*2). */
    for (idx_t d = min_factor; d <= n / d; ++ d) {
        if ((n % d == 0) && allowed[d]) {
            result += count_factorizations(n / d, d);
        }
    }

    /* Store in cache */
    hash_table[pos].used = 1;
    hash_table[pos].key  = key;
    hash_table[pos].val  = result;

    return result;
}

/* ------------------------------------------------------------------ */
/* Public sequence function.                                          */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 1) return 1;   /* by definition a(1)=1 */

    /* Initialise global state on the first call (or when n grows). */
    static int initialized = 0;
    if (!initialized) {
        initialized = 1;
        hash_table = (HashEntry *)calloc(hash_size, sizeof(HashEntry));
        if (!hash_table) abort();    /* memory error */
        init_allowed(n);
    } else if (n > allowed_size) {
        init_allowed(n);
    }

    return count_factorizations(n, 2);
}
