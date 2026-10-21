/*
 * A291063  Number of maximal irredundant sets in the n-wheel graph.
 * https://oeis.org/A291063
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A286954(n-1) + 1. Using the linear recurrence of order 15
 *            derived from the generating function: a(n) = a(n-1) + a(n-2) +
 *            a(n-9) + a(n-15) + 3*a(n-10) - a(n-6) - a(n-7) - a(n-8) -
 *            a(n-11) - a(n-12) - a(n-14) for all n > 16. The first 15 values
 *            (a(2) … a(16)) are hard‑coded for initialisation
 * Range:     u8 n <= 15, u16 n <= 29, u32 n <= 58, u64 n <= 116,
 *            u128 n <= 231, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* ------------------------------------------------------------------ */
/*  Base values for n = 0 … 16 (offset 2: a(2)=1).                    */
static const seq_t base[17] = {
    0ULL,          /* a(0) – unused           */
    0ULL,          /* a(1) – unused           */
    1ULL,          /* a(2)                    */
    3ULL,          /* a(3)                    */
    4ULL,          /* a(4)                    */
    7ULL,          /* a(5)                    */
   11ULL,          /* a(6)                    */
   12ULL,          /* a(7)                    */
   15ULL,          /* a(8)                    */
   15ULL,          /* a(9)                    */
   31ULL,          /* a(10)                   */
   63ULL,          /* a(11)                   */
   67ULL,          /* a(12)                   */
  100ULL,          /* a(13)                   */
  144ULL,          /* a(14)                   */
  213ULL,          /* a(15)                   */
  344ULL           /* a(16)                   */
};

/* ------------------------------------------------------------------ */
/*  Dynamic storage for computed values.                               */
static seq_t *dp      = NULL;   /* array of a(n) values */
static size_t dp_size = 0;      /* allocated size */
static idx_t   last   = 16;     /* highest n computed */

/*  Ensure the dynamic array holds at least indices 0 … n.           */
static void
ensure_capacity(idx_t n)
{
    size_t needed = (size_t)(n + 1);
    if (dp_size >= needed) return;

    size_t new_size = dp_size ? dp_size << 1 : 16;
    while (new_size < needed) new_size <<= 1;

    seq_t *tmp = (seq_t *)realloc(dp, new_size * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);          /* abort on OOM */
    /* initialise new slots to 0 – not strictly needed for known terms */
    for (size_t i = dp_size; i < new_size; ++i) tmp[i] = 0;
    dp = tmp;
    dp_size = new_size;
}

/*  Compute a(n) for n ≥ 2.  Uses memoisation.                        */
seq_t
a(idx_t n)
{
    if (n < 2) return 0;          /* values below offset are undefined */

    if (n <= 16) return base[n];

    /* initialise dynamic storage on first call */
    if (!dp) {
        ensure_capacity(n);
        for (idx_t i = 0; i <= 16; ++i) dp[i] = base[i];
        last = 16;
    }

    if (n > last) {
        ensure_capacity(n);

        for (idx_t i = last + 1; i <= n; ++i) {
            seq_t val = 0;

            /* non‑negative contributions */
            val += dp[i-1];          /* a(n-1)   */
            val += dp[i-2];          /* a(n-2)   */
            val += dp[i-9];          /* a(n-9)   */
            val += dp[i-15];         /* a(n-15)  */
            val += dp[i-10] * 3;     /* 3*a(n-10)*/

            /* negative contributions */
            val -= dp[i-6];          /* a(n-6)   */
            val -= dp[i-7];          /* a(n-7)   */
            val -= dp[i-8];          /* a(n-8)   */
            val -= dp[i-11];         /* a(n-11)  */
            val -= dp[i-12];         /* a(n-12)  */
            val -= dp[i-14];         /* a(n-14)  */

            dp[i] = val;
        }
        last = n;
    }
    return dp[n];
}
