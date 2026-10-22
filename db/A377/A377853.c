/*
 * A377853  Sum over all multiplicative partitions mu of n whose sum is also n
 *          (with factors >= 1), of the encoding as Product_{j in mu}
 *          prime(j).
 * https://oeis.org/A377853
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum over all multiplicative partitions μ of n whose sum
 *            equals n, of Product_{j in μ} prime(j)
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 35, u64 n <= 69, u128 n <= 134,
 *            big any n
 * Helpers:   get_prime, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Required helper functions */
#include "../utils/get_prime.h"   /* get_prime(idx_t) -> uint32_t */
#include "../utils/pow_uint.h"    /* pow_uint(uint64_t, idx_t) -> seq_t */

/* Cache for already computed values */
static seq_t *cache = NULL;   /* cache[n] holds a(n), except cache[0] */
static idx_t cache_cap = 0;   /* number of allocated cache slots */

/* Expand the cache to accommodate at least indices [0..n].  New
   entries are initialised to 0.  */
static void ensure_cache(idx_t n)
{
    if (n < cache_cap)
        return;

    idx_t new_cap = cache_cap ? cache_cap : 64;
    while (new_cap <= n)
        new_cap <<= 1;

    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) abort();

    for (idx_t i = cache_cap; i < new_cap; ++i)
        new_cache[i] = 0;

    cache = new_cache;
    cache_cap = new_cap;
}

/* Recursive enumeration of factorizations of `remaining` into factors
   >= `start`.  `sum_so_far` is the sum of the factors chosen so far.
   `enc` holds the current product of prime(k) for the chosen factors.
   When the product reaches 1 the partition is complete and the
   remaining ones are added to satisfy the sum constraint. */
static void rec(idx_t remaining, idx_t start, idx_t sum_so_far,
                seq_t enc, seq_t *result, idx_t orig_n)
{
    if (remaining == 1) {
        /* all non‑1 factors chosen; count how many 1's are needed */
        idx_t ones = orig_n - sum_so_far;            /* ones ≥ 0 */
        seq_t term = enc * pow_uint(2, ones);        /* prime(1)=2 */
        *result += term;
        return;
    }

    for (idx_t d = start; d <= remaining; ++d) {
        if (remaining % d != 0) continue;
        idx_t new_sum = sum_so_far + d;
        if (new_sum > orig_n) continue;             /* cannot match sum later */
        seq_t new_enc = enc * get_prime(d);
        rec(remaining / d, d, new_sum, new_enc, result, orig_n);
    }
}

/* Main function requested by the test harness */
seq_t a(idx_t n)
{
    if (n == 1u) return 2;  /* a(1) = prime(1) = 2 */

    ensure_cache(n);
    if (cache[n] != 0) return cache[n];

    seq_t total = 0;
    rec(n, 2, 0, 1, &total, n);

    cache[n] = total;
    return total;
}
