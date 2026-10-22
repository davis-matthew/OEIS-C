/*
 * A394218  Earliest monotonic sequence satisfying a(1)=1 and
 *          a(a(n)+a(n-1)+a(n-2)) = n for n>=1 (with a(k)=0 for k<=0).
 * https://oeis.org/A394218
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1, a(2)=2, a(3)=2, a(4)=2, a(5)=3, \ for k ≥ 2: \ a(3k) =
 *            a(k-2)+a(k-1)+a(k)+1+ c(k-1) \ a(3k+1) = a(k-1)+a(k)+a(k+1) \
 *            a(3k+2) = a(k)+a(k+1)+a(k+2)-1- c(k) \ with c(m)=0 for m<5,
 *            c(5)=1, and for j≥2: c(3j)=c(j-1), c(3j+1)=c(3j+2)=c(j)
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* static storage for the sequence and auxiliary data */
static seq_t *seq = NULL;          /* 0‑based array of a(n) */
static idx_t  seq_len  = 0;        /* allocated size of seq */
static idx_t  max_computed = 0;    /* largest n already computed */

static unsigned char *carr = NULL; /* 0/1 values for c(m) */
static idx_t  carr_len = 0;        /* allocated length of carr */

static void ensure_seq_capacity(idx_t need)
{
    if (need <= seq_len)
        return;
    size_t new_len = seq_len ? seq_len * 2 : 64;
    while (new_len < need)
        new_len *= 2;
    seq = (seq_t *)realloc(seq, new_len * sizeof(seq_t));
    seq_len = new_len;
}

static void init_seq(void)
{
    if (seq != NULL)
        return;
    seq_len = 64;
    seq = (seq_t *)malloc(seq_len * sizeof(seq_t));
    /* a(0)=0 for convenience, a(1)=1, ... a(5)=3 */
    seq[0] = (seq_t)0;
    seq[1] = (seq_t)1;
    seq[2] = (seq_t)2;
    seq[3] = (seq_t)2;
    seq[4] = (seq_t)2;
    seq[5] = (seq_t)3;
    max_computed = 5;
}

static void ensure_carr(idx_t m)
{
    if (m < carr_len)
        return;
    idx_t old_len = carr_len;
    idx_t new_len = m + 1;
    carr = (unsigned char *)realloc(carr, new_len * sizeof(unsigned char));
    carr_len = new_len;
    for (idx_t i = old_len; i <= m; ++i) {
        if (i < 5) {
            carr[i] = 0;
        } else if (i == 5) {
            carr[i] = 1;
        } else {
            if (i % 3 == 0) {
                carr[i] = carr[i / 3 - 1];
            } else if (i % 3 == 1) {
                carr[i] = carr[(i - 1) / 3];
            } else {
                carr[i] = carr[(i - 2) / 3];
            }
        }
    }
}

static inline unsigned char get_c(idx_t m)
{
    ensure_carr(m);
    return carr[m];
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* safety, not part of the sequence */

    init_seq();

    if (n <= max_computed)
        return seq[n];

    /* determine the maximum k needed to reach index n */
    idx_t limit_k = n / 3;
    if (limit_k == 0)
        limit_k = 1;              /* we need at least k=1 for proper indices */

    /* ensure capacity for the largest index that may be written */
    idx_t need = 3 * limit_k + 3;  /* need up to 3k+2 */
    if (need < n + 1)
        need = n + 1;
    ensure_seq_capacity(need);

    ensure_carr(limit_k);

    /* compute recurrence for k from current maximum up to limit_k */
    idx_t start_k = (max_computed >= 5) ? (max_computed / 3) : 1;
    for (idx_t k = start_k + 1; k <= limit_k; ++k) {
        idx_t i3 = 3 * k;
        seq_t v3k  = seq[k - 2] + seq[k - 1] + seq[k] + 1 + get_c(k - 1);
        seq[i3]    = v3k;
        seq_t v3k1 = seq[k - 1] + seq[k] + seq[k + 1];
        seq[i3 + 1] = v3k1;
        seq_t v3k2 = seq[k] + seq[k + 1] + seq[k + 2] - 1 - get_c(k);
        seq[i3 + 2] = v3k2;
    }

    /* update the highest computed index */
    if ((3 * limit_k + 2) > max_computed)
        max_computed = 3 * limit_k + 2;

    return seq[n];
}
