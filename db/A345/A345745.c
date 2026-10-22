/*
 * A345745  a(n) = Sum_{k=1..n} n^(1 - mu(k)^2).
 * https://oeis.org/A345745
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} n^(1 - mu(k)^2)
 * Range:     u8 n <= 26, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements a(n) = n^2 - (n-1)*Q(n)  where Q(n) is the count of square‑free integers ≤ n. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static idx_t cur_cap = 0;
static int32_t *spf = NULL;          /* smallest prime factor */
static uint8_t *is_sqfree = NULL;    /* 1 if square‑free, 0 otherwise */
static idx_t *prefix_sqfree = NULL;  /* prefix sums of square‑free counts */

static void ensure_capacity(idx_t n)
{
    if (n + 1 <= cur_cap)
        return;

    idx_t new_cap = n + 1;
    int32_t *new_spf = (int32_t *)malloc(new_cap * sizeof(int32_t));
    uint8_t *new_sqfree = (uint8_t *)malloc(new_cap * sizeof(uint8_t));
    idx_t *new_prefix = (idx_t *)malloc(new_cap * sizeof(idx_t));
    if (!new_spf || !new_sqfree || !new_prefix)
        abort();

    /* initialise spf array */
    for (idx_t i = 0; i < new_cap; ++i)
        new_spf[i] = 0;
    if (new_cap > 1)
        new_spf[1] = 1;
    for (idx_t i = 2; i < new_cap; ++i) {
        if (!new_spf[i]) {
            new_spf[i] = (int32_t)i;
            if ((uint64_t)i * (uint64_t)i < new_cap) {
                for (idx_t j = i * i; j < new_cap; j += i)
                    if (!new_spf[j])
                        new_spf[j] = (int32_t)i;
            }
        }
    }

    /* compute square‑free flag and prefix sum */
    new_prefix[0] = 0;
    for (idx_t i = 1; i < new_cap; ++i) {
        bool squarefree = true;
        idx_t temp = i;
        while (temp > 1) {
            int32_t p = new_spf[temp];
            int  cnt = 0;
            while (temp % p == 0) {
                ++cnt;
                temp /= p;
                if (cnt > 1) {
                    squarefree = false;
                    break;
                }
            }
            if (!squarefree)
                break;
        }
        new_sqfree[i] = squarefree ? 1 : 0;
        new_prefix[i] = new_prefix[i - 1] + new_sqfree[i];
    }

    /* free old arrays */
    free(spf);
    free(is_sqfree);
    free(prefix_sqfree);

    /* store new arrays */
    spf = new_spf;
    is_sqfree = new_sqfree;
    prefix_sqfree = new_prefix;
    cur_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;          /* undefined for n < 1, return 0 as sentinel */

    ensure_capacity(n);

    idx_t Q = prefix_sqfree[n];   /* count of square‑free ≤ n */
    seq_t nn = (seq_t)n;
    seq_t Qs = (seq_t)Q;

    /* a = n^2 - (n-1)*Q */
    return nn * nn - (nn - (seq_t)1) * Qs;
}
