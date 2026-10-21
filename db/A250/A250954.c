/*
 * A250954  Number of (n+1)X(6+1) 0..1 arrays with no 2X2 subblock having its
 *          maximum diagonal element less than its minimum antidiagonal
 *          element.
 * https://oeis.org/A250954
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) follows the empirical 9‑term linear recurrence a(n) =
 *            103*a(n-1) -1452*a(n-2) +1999*a(n-3) +25412*a(n-4) -90159*a(n-5)
 *            +84285*a(n-6) +261*a(n-7) -20412*a(n-8) +4212*a(n-9) (n ≥ 10)
 * Range:     u8 none, u16 n <= 1, u32 n <= 3, u64 n <= 8, u128 n <= 18,
 *            big any n
 * Helpers:   seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/seq_from_str.h"

static const int coeffs[9] = {
    103, -1452, 1999, 25412, -90159, 84285, 261, -20412, 4212
};

static seq_t *cache = NULL;           /* dynamic cache for computed terms   */
static size_t cache_cap = 0;          /* allocated capacity in elements     */
static size_t cache_len = 0;          /* number of terms cached (≥0)         */

/* Ensure the cache can hold at least 'need' elements */
static void ensure_cache_capacity(size_t need)
{
    if (need <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    if (new_cap < need) new_cap = need;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();  /* allocation failure */
    cache = tmp;
    cache_cap = new_cap;
}

/* Initialise the cache with the first nine known terms */
static void init_cache(void)
{
    if (cache_len != 0) return;   /* already initialised */
    ensure_cache_capacity(9);
    /* Use seq_from_str to avoid overflow in the literal values */
    cache[0] = seq_from_str("10864");
    cache[1] = seq_from_str("941192");
    cache[2] = seq_from_str("81431944");
    cache[3] = seq_from_str("7045780240");
    cache[4] = seq_from_str("609622239024");
    cache[5] = seq_from_str("52746349035592");
    cache[6] = seq_from_str("4563772381621072");
    cache[7] = seq_from_str("394871276236514256");
    cache[8] = seq_from_str("34165447249322574280");
    cache_len = 9;
}

/* Compute and cache terms up to (and including) index n (1‑based) */
static void compute_terms_up_to(idx_t n)
{
    if (n == 0) return;           /* undefined for n==0 */
    init_cache();
    size_t target = (size_t)n;    /* convert to 0‑based array length */
    if (target <= cache_len) return; /* already computed */

    ensure_cache_capacity(target);

    for (size_t i = cache_len; i < target; ++i) {
        seq_t acc = 0;                      /* accumulator (signed seq_t) */
        /* indices: cache[i-1] = a(i), ..., cache[i-9] = a(i-8) */
        for (int k = 0; k < 9; ++k) {
            seq_t prod = cache[i - 1 - k] *
                         (seq_t)(coeffs[k] > 0 ? coeffs[k] : -coeffs[k]);
            if (coeffs[k] < 0)
                acc -= prod;
            else
                acc += prod;
        }
        cache[i] = acc;
    }
    cache_len = target;
}

seq_t a(idx_t n)
{
    compute_terms_up_to(n);
    return cache[(size_t)n - 1];
}
