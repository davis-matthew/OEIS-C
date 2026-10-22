/*
 * A302754  Maximum remainder of prime(p) + prime(q) divided by p + q with p
 *          <= q <= n.
 * https://oeis.org/A302754
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 154, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A302754
    a(n) = max{ (prime(p)+prime(q)) mod (p+q) | 1 <= p <= q <= n }
    (prime(i) is the i-th prime number, 1‑based)
*/

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* ------------------------------------------------------------------ */
/* Helper to allocate a dynamic array of seq_t that is safe in C and C++ */
/* ------------------------------------------------------------------ */
static void grow_seq_array(seq_t **ptr, size_t *cap, size_t need)
{
#ifdef __cplusplus
    if (*cap >= need) return;
    size_t new_cap = (*cap == 0) ? 16 : (*cap << 1);
    while (new_cap < need) new_cap <<= 1;
    seq_t *new_arr = new seq_t[new_cap]();      /* default‑constructed */
    if (*ptr) {
        for (size_t i = 0; i < *cap; ++i) new_arr[i] = (*ptr)[i];
        delete[] *ptr;
    }
    *ptr = new_arr;
    *cap = new_cap;
#else
    if (*cap >= need) return;
    size_t new_cap = (*cap == 0) ? 16 : (*cap << 1);
    while (new_cap < need) new_cap <<= 1;
    seq_t *new_arr = (seq_t *)realloc(*ptr, new_cap * sizeof(seq_t));
    if (!new_arr) abort();
    *ptr = new_arr;
    *cap = new_cap;
#endif
}

/* ------------------------------------------------------------------ */
/* Prime generation (trial division) – fast enough for n <= 10000   */
/* ------------------------------------------------------------------ */
static uint64_t *prime_vals   = NULL;   /* prime_vals[i-1] == prime(i)      */
static size_t   prime_cap    = 0;       /* capacity of prime_vals           */
static size_t   prime_count  = 0;       /* number of primes stored          */

static void grow_prime_vector(size_t need)
{
    if (prime_cap >= need) return;
    size_t new_cap = (prime_cap == 0) ? 16 : (prime_cap << 1);
    while (new_cap < need) new_cap <<= 1;
    uint64_t *tmp;
    tmp = (uint64_t *)realloc(prime_vals, new_cap * sizeof(uint64_t));
    if (!tmp) abort();
    prime_vals = tmp;
    prime_cap  = new_cap;
}

static void ensure_primes_upto(idx_t n)
{
    if (prime_count >= (size_t)n) return;

    /* Resize vector to hold at least n primes */
    grow_prime_vector((size_t)n);

    if (prime_count == 0) {
        prime_vals[prime_count++] = 2;   /* prime(1) */
    }

    uint64_t cand = prime_vals[prime_count - 1] + 1;
    if ((cand & 1ULL) == 0) cand++;          /* make odd */

    while (prime_count < (size_t)n) {
        bool is_prime = true;
        for (size_t i = 0; i < prime_count; ++i) {
            uint64_t p = prime_vals[i];
            if (p * p > cand) break;
            if (cand % p == 0) { is_prime = false; break; }
        }
        if (is_prime) prime_vals[prime_count++] = cand;
        cand += 2;                           /* only odd candidates */
    }
}

/* ------------------------------------------------------------------ */
/* Cache for a(n) values                                                  */
/* ------------------------------------------------------------------ */
static seq_t *a_cache   = NULL;   /* a_cache[i] holds a(i) */
static size_t a_cap    = 0;       /* capacity of a_cache          */
static idx_t  a_len    = 0;       /* highest index computed       */

static void grow_a_cache(idx_t need)
{
    grow_seq_array(&a_cache, &a_cap, (size_t)need + 1); /* +1 for 1‑based index */
}

/* ------------------------------------------------------------------ */
/* Compute a(n) incrementally                                           */
/* ------------------------------------------------------------------ */
static void ensure_a_up_to(idx_t n)
{
    if (a_len >= n) return;                  /* already computed */

    /* Ensure primes for indices up to n */
    ensure_primes_upto(n);

    /* Grow cache to hold up to n */
    grow_a_cache(n);

    /* Incrementally compute a(n) for new indices */
    for (idx_t i = a_len + 1; i <= n; ++i) {
        seq_t max_rem = (i == 1) ? 0 : a_cache[i - 1];   /* previous maximum */
        uint64_t pi = prime_vals[i - 1];                 /* prime(i) */

        for (idx_t p = 1; p <= i; ++p) {
            uint64_t pp = prime_vals[p - 1];
            uint64_t sum64 = pp + pi;                    /* prime(p)+prime(i) fits in 64‑bit */
            seq_t rem = (seq_t)(sum64 % (p + i));        /* modulo in 64‑bit, cast to seq_t */
            if (rem > max_rem) max_rem = rem;
        }
        a_cache[i] = max_rem;
    }
    a_len = n;
}

/* ------------------------------------------------------------------ */
/* Public function                                                   */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n < 1) return 0;            /* undefined offset, return 0 */
    ensure_a_up_to(n);
    return a_cache[n];
}
