/*
 * A331489  Matula-Goebel numbers of topologically series-reduced rooted
 *          trees.
 * https://oeis.org/A331489
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A331489 – Matula-Goebel numbers of topologically series‑reduced rooted trees
 *  Formula: a(n) is the n‑th smallest integer whose prime factorisation
 *           p_a1 * p_a2 * … * p_ak (with repetitions allowed) satisfies
 *           1) each index ai is itself a valid interior number, and
 *           2) k ≠ 2  (root nodes are not allowed to have exactly two children).
 *  The implementation enumerates positive integers in ascending order,
 *  tests the factorisation against the interior set, and collects the
 *  admissible root numbers until the required index n is reached.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/*------------------------------------------------------------*/
/*  Prime sieve (once, for all calls)                        */
/*------------------------------------------------------------*/
#define PRIME_SIEVE_LIMIT 20000000U   /* 20 million – ample for the required range */

static uint32_t *prime_list   = NULL;   /* primes in ascending order */
static size_t   prime_cnt    = 0;

/* binary search for the 1‑based prime index – assumes p is prime */
static size_t prime_index(uint32_t p)
{
    size_t lo = 0, hi = prime_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        uint32_t v = prime_list[mid];
        if (v == p)
            return mid + 1;          /* convert to 1‑based index */
        if (v < p)
            lo = mid + 1;
        else
            hi = mid;
    }
    return 0;  /* not found – should never happen for a prime in the table */
}

static void init_primes(void)
{
    if (prime_list != NULL)
        return;   /* already initialised */

    char *is_comp = (char *)calloc(PRIME_SIEVE_LIMIT + 1, 1);
    if (!is_comp) abort();

    size_t lim = PRIME_SIEVE_LIMIT;
    size_t sq  = (size_t) sqrt((double)lim);
    for (size_t i = 2; i <= sq; ++i) {
        if (!is_comp[i]) {
            for (size_t j = i * i; j <= lim; j += i)
                is_comp[j] = 1;
        }
    }

    /* count primes to allocate array */
    prime_cnt = 0;
    for (size_t i = 2; i <= lim; ++i)
        if (!is_comp[i]) ++prime_cnt;

    prime_list = (uint32_t *)malloc(prime_cnt * sizeof(uint32_t));
    if (!prime_list) abort();

    size_t idx = 0;
    for (size_t i = 2; i <= lim; ++i)
        if (!is_comp[i])
            prime_list[idx++] = (uint32_t)i;

    free(is_comp);
}

/*------------------------------------------------------------*/
/*  Dynamic array for the interior set  (is_in_I)            */
/*------------------------------------------------------------*/
static uint8_t *is_in_I  = NULL;   /* 0 = false, 1 = true */
static size_t  is_sz    = 0;       /* current allocated size */

/* ensure the array can hold index 'idx' (0‑based) */
static void ensure_is_in_I(idx_t idx)
{
    if ((size_t)idx + 1 <= is_sz)
        return;
    size_t new_sz = is_sz ? is_sz * 2 : 1;
    while (new_sz <= (size_t)idx + 1)
        new_sz <<= 1;

    uint8_t *tmp = (uint8_t *)realloc(is_in_I, new_sz * sizeof(uint8_t));
    if (!tmp) abort();
    memset(tmp + is_sz, 0, (new_sz - is_sz));
    is_in_I = tmp;
    is_sz   = new_sz;
}

/*------------------------------------------------------------*/
/*  Dynamic array for the root value sequence                */
/*------------------------------------------------------------*/
static idx_t *root_vals      = NULL;   /* values in ascending order */
static size_t root_cap        = 0;     /* capacity of array */
static size_t root_len        = 0;     /* number of stored terms */

/* ensure capacity for at least 'need' elements */
static void ensure_root_vals(size_t need)
{
    if (root_cap >= need)
        return;
    size_t new_cap = root_cap ? root_cap * 2 : 16;
    while (new_cap < need)
        new_cap <<= 1;
    idx_t *tmp = (idx_t *)realloc(root_vals, new_cap * sizeof(idx_t));
    if (!tmp) abort();
    root_vals = tmp;
    root_cap  = new_cap;
}

/*------------------------------------------------------------*/
/*  Sequence function                                        */
/*------------------------------------------------------------*/
seq_t a(idx_t n)
{
    static int  init_done = 0;
    static idx_t next_candidate = 2;  /* next integer to test */

    if (!init_done) {
        /* initialise data structures */
        init_primes();

        /* leaf 1 is an interior node */
        ensure_is_in_I(1);
        is_in_I[1] = 1;

        /* 1 is also a valid root number */
        ensure_root_vals(1);
        root_vals[0] = 1;
        root_len = 1;

        init_done = 1;
    }

    /* if we already have enough terms, return immediately */
    if (root_len >= n)
        return (seq_t)root_vals[n - 1];

    /* continue searching from the last tested integer */
    idx_t cur = next_candidate;
    while (root_len < n) {
        idx_t temp = cur;
        size_t child_cnt = 0;
        int    ok = 1;    /* flag whether all child indices are interior */

        /* trial division using precomputed primes */
        for (size_t pi = 0; pi < prime_cnt && (uint64_t)prime_list[pi] * (uint64_t)prime_list[pi] <= temp; ++pi) {
            uint32_t p = prime_list[pi];
            if (temp % p == 0) {
                size_t exp = 0;
                while (temp % p == 0) {
                    temp /= p;
                    ++exp;
                }
                child_cnt += exp;
                size_t kid = prime_index(p);   /* child index (1‑based) */
                if (kid == 0 || !is_in_I[kid]) {
                    ok = 0;
                    break;
                }
            }
        }

        if (ok && temp > 1) {  /* remaining prime factor */
            uint32_t p = (uint32_t)temp;
            child_cnt++;
            size_t kid = prime_index(p);
            if (kid == 0 || !is_in_I[kid])
                ok = 0;
        }

        if (ok) {
            int is_interior = (child_cnt != 1); /* interior nodes cannot have exactly one child */
            int is_root     = (child_cnt != 2); /* root cannot have exactly two children */

            if (is_interior) {
                ensure_is_in_I(cur);
                is_in_I[(size_t)cur] = 1;
            }
            if (is_root) {
                ensure_root_vals(root_len + 1);
                root_vals[root_len++] = cur;
            }
        }

        ++cur;
    }

    next_candidate = cur; /* next integer to test for future calls */
    return (seq_t)root_vals[n - 1];
}
