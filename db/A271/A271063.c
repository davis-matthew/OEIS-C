/*
 * A271063  a(1) = 2, a(2) = 3; thereafter a(n) = a(n-1) + a(|n-a(T)|), where
 *          a(T) is the largest term in the sequence before a(n) such that 0 <
 *          |n-a(T)| < n.
 * https://oeis.org/A271063
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 44, u32 n <= 212, u64 n <= 678,
 *            u128 n <= 1982, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Recurrence used for OEIS A271063  
    a(1) = 2, a(2) = 3; for n >= 3  
      a(n) = a(n-1) + a(|n - a(T)|)  
    where T is the largest index < n such that a(T) < 2n and a(T) != n.
    The sequence is strictly increasing, so the first such T found when
    scanning backwards gives the required largest a(T).  */

#include <stdlib.h>

static seq_t *seq = NULL;        /* array of computed terms, seq[0] = 0 */
static size_t seq_cap = 0;       /* number of valid indices stored (>= seq_cap) */
static idx_t seq_len = 0;        /* highest index already computed */

/* Ensure capacity for indices up to n (inclusive). */
static void ensure_cap(idx_t n)
{
    if ((size_t)n < seq_cap)
        return;

    size_t new_cap = seq_cap ? seq_cap * 2 : 16;
    while (new_cap <= (size_t)n)
        new_cap <<= 1;

    seq_t *tmp;
    if (seq == NULL)
        tmp = (seq_t *)malloc((new_cap + 1) * sizeof(seq_t));
    else
        tmp = (seq_t *)realloc(seq, (new_cap + 1) * sizeof(seq_t));

    if (tmp == NULL) { /* in case of out‑of‑memory, leave as is */
        return;
    }
    seq = tmp;
    seq_cap = new_cap;
    seq[0] = 0;            /* dummy entry */
}

/* Main sequence routine */
seq_t a(idx_t n)
{
    if (n == 0)                /* undefined, but return 0 for safety */
        return 0;

    if (n > seq_len) {
        ensure_cap(n);
        for (idx_t i = seq_len + 1; i <= n; ++i) {
            if (i == 1) {
                seq[1] = (seq_t)2;
                continue;
            }
            if (i == 2) {
                seq[2] = (seq_t)3;
                continue;
            }

            /* find largest T < i with a(T) < 2i and a(T)!=i            */
            idx_t T = i - 1;
            seq_t two_i = (seq_t)i;
            two_i += two_i;       /* two_i = 2*i in seq_t */

            while (T > 0) {
                seq_t val = seq[T];
                if (val < two_i && val != (seq_t)i)
                    break;
                --T;
            }

            /* diff = |i - a(T)|, guaranteed >0 and < i */
            seq_t aT = seq[T];
            seq_t diff_val = (aT > (seq_t)i) ? aT - (seq_t)i
                                              : (seq_t)i - aT;
            idx_t diff = (idx_t)diff_val;   /* safe: diff < i */

            seq[i] = seq[i - 1] + seq[diff];
        }
        seq_len = n;
    }
    return seq[n];
}
