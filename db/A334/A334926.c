/*
 * A334926  G.f.: Sum_{k>=1} x^(k*(2*k^2 + 1)/3) / (1 - x^(k*(2*k^2 + 1)/3)).
 * https://oeis.org/A334926
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of octahedral numbers (A005900) dividing n
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/*
 * Static cache for octahedral numbers.
 * oct_nums holds the octahedral numbers in ascending order.
 * oct_cnt  – number of stored numbers.
 * oct_cap  – current capacity of the allocated array.
 * max_cached_n – the largest n for which the cache is valid.
 */
static seq_t *oct_nums = NULL;
static size_t  oct_cnt  = 0;
static size_t  oct_cap  = 0;
static idx_t   max_cached_n = 0;

/* Ensure that all octahedral numbers <= n are stored in oct_nums. */
static void
ensure_oct_cache(idx_t n)
{
    if (n <= max_cached_n)
        return;  /* already cached up to n */

    /* Start from the next k after the last stored number */
    idx_t k_start = (idx_t)(oct_cnt + 1); /* since O_k for k>=1 */

    for (idx_t k = k_start; ; ++k) {
        seq_t kk = (seq_t)k;
        /* Compute O_k = k * (2*k^2 + 1) / 3 */
        seq_t tmp = ((kk * kk) * 2 + (seq_t)1);
        seq_t oct = (kk * tmp) / (seq_t)3;

        /* Stop when the next octahedral number exceeds n */
        if ((seq_t)n < oct)
            break;

        /* Grow array if needed */
        if (oct_cnt >= oct_cap) {
            size_t new_cap = (oct_cap == 0) ? 16 : oct_cap << 1;
            seq_t *tmp_ptr = (seq_t *)realloc(oct_nums, sizeof(seq_t) * new_cap);
            if (!tmp_ptr)
                abort();  /* out‑of‑memory */
            oct_nums = tmp_ptr;
            oct_cap  = new_cap;
        }

        oct_nums[oct_cnt++] = oct;
    }

    max_cached_n = n;
}

/* Main function required by the build system */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;  /* undefined for n=0, but return 0 for safety */

    /* Make sure cache covers all octahedral numbers <= n */
    ensure_oct_cache(n);

    seq_t N   = (seq_t)n;
    seq_t cnt = (seq_t)0;

    /* Count how many stored octahedral numbers divide N */
    for (size_t i = 0; i < oct_cnt; ++i) {
        seq_t oct = oct_nums[i];
        if (oct > N)
            break;
        if (N % oct == (seq_t)0)
            ++cnt;
    }

    return cnt;
}
