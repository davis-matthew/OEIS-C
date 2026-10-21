/*
 * A287120  Number of non-attacking bishop positions on a 3 X n chessboard.
 * https://oeis.org/A287120
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=8, …, a(8)=24649 For n ≥ 9: a(n) = 3*a(n‑1) +
 *            2*a(n‑3) + 4*a(n‑4) –10*a(n‑5) – 2*a(n‑6) – a(n‑8) + a(n‑9)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 18, u64 n <= 37, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *seq = NULL;   /* cached sequence values                     */
static size_t sz   = 0;     /* number of values currently stored          */
static size_t cap  = 0;     /* allocated capacity                         */

/* initialise cache with the first nine exact terms */
static void init_seq(void)
{
    if (!seq) {
        cap = 32;                      /* initial capacity                 */
        seq = (seq_t *)malloc(cap * sizeof(seq_t));

        /* exact values for n = 0 … 8 */
        seq[0] = 1;
        seq[1] = 8;
        seq[2] = 25;
        seq[3] = 70;
        seq[4] = 225;
        seq[5] = 748;
        seq[6] = 2401;
        seq[7] = 7668;
        seq[8] = 24649;
        sz = 9;
    }
}

/* ensure that seq[n] is computed and cached */
static void ensure_size(idx_t n)
{
    init_seq();
    size_t idx = (size_t)n;
    if (idx < sz) return;                 /* already available */

    while (sz <= idx) {
        if (sz >= cap) {                  /* grow storage */
            cap *= 2;
            seq = (seq_t *)realloc(seq, cap * sizeof(seq_t));
        }

        /* recurrence for next value */
        idx_t i = sz;
        seq[i] = 3 * seq[i-1];
        seq[i] += 2 * seq[i-3];
        seq[i] += 4 * seq[i-4];
        seq[i] -= 10 * seq[i-5];
        seq[i] -= 2 * seq[i-6];
        seq[i] -= seq[i-8];
        seq[i] += seq[i-9];
        ++sz;
    }
}

/* external linkage, required function */
seq_t a(idx_t n)
{
    ensure_size(n);
    return seq[(size_t)n];
}
