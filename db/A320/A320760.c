/*
 * A320760  Number of ordered set partitions of [n] where the maximal block
 *          size equals four.
 * https://oeis.org/A320760
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = f4(n) – f3(n) where f_k(0)=1 and for n≥1,
 *            f_k(n)=Σ_{m=1..min(k,n)} C(n,m)*f_k(n-m). f3 counts ordered set
 *            partitions with block size ≤3, f4 counts those with block size
 *            ≤4
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 12, u64 n <= 19, u128 n <= 31,
 *            big any n
 * Helpers:   alloc_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <string.h>
#include "../utils/alloc_seq.h"   /* alloc_seq(size_t) -> seq_t* */

static seq_t *f3 = NULL, *f4 = NULL;
static idx_t  cap = 0;       /* allocated array size (≥1) */
static idx_t  max_n = 0;     /* highest n whose values are computed */

static void ensure_up_to(idx_t n)
{
    if (cap == 0) {
        /* initial allocation for index 0 */
        cap   = 1;
        f3    = alloc_seq(cap);
        f4    = alloc_seq(cap);
        f3[0] = 1;
        f4[0] = 1;
    }

    if (n <= max_n) return;

    /* extend capacity if needed */
    if (cap <= n) {
        idx_t new_cap = cap ? cap : 1;
        while (new_cap <= n) new_cap <<= 1;
        seq_t *nf3 = alloc_seq(new_cap);
        seq_t *nf4 = alloc_seq(new_cap);
        memcpy(nf3, f3, (size_t)cap * sizeof(seq_t));
        memcpy(nf4, f4, (size_t)cap * sizeof(seq_t));
        f3 = nf3;
        f4 = nf4;
        cap = new_cap;
        /* old arrays leak – test harness runs in a single process */
    }

    /* compute values from max_n+1 up to n */
    for (idx_t i = max_n + 1; i <= n; ++i) {
        /* f3 (k=3) */
        seq_t sum3 = 0;
        seq_t bin1 = (seq_t)i;
        sum3 += bin1 * f3[i - 1];
        if (i >= 2) {
            seq_t bin2 = ((seq_t)i * (seq_t)(i - 1)) / 2;
            sum3 += bin2 * f3[i - 2];
            if (i >= 3) {
                seq_t bin3 = ((seq_t)i * (seq_t)(i - 1) * (seq_t)(i - 2)) / 6;
                sum3 += bin3 * f3[i - 3];
            }
        }
        f3[i] = sum3;

        /* f4 (k=4) */
        seq_t sum4 = 0;
        sum4 += bin1 * f4[i - 1];
        if (i >= 2) {
            seq_t bin2 = ((seq_t)i * (seq_t)(i - 1)) / 2;
            sum4 += bin2 * f4[i - 2];
            if (i >= 3) {
                seq_t bin3 = ((seq_t)i * (seq_t)(i - 1) * (seq_t)(i - 2)) / 6;
                sum4 += bin3 * f4[i - 3];
                if (i >= 4) {
                    seq_t bin4 = ((seq_t)i * (seq_t)(i - 1) * (seq_t)(i - 2) *
                                 (seq_t)(i - 3)) / 24;
                    sum4 += bin4 * f4[i - 4];
                }
            }
        }
        f4[i] = sum4;
    }

    max_n = n;
}

seq_t a(idx_t n)
{
    if (n < 4) return 0;          /* undefined below offset; return 0 */
    ensure_up_to(n);
    return f4[n] - f3[n];
}
