/*
 * A356998  a(n) = b(n) - b(n - b(n)) for n >= 2, where b(n) = A356988(n).
 * https://oeis.org/A356998
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 92, u16 n <= 92, u32 n <= 92, u64 n <= 92, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A356998
   a(n) = b(n) – b(n–b(n)),  n ≥ 2,  where
   b(n) is given by the Fibonacci/Lucas formulas described in the OEIS entry.
   The function below evaluates a(n) directly from those formulas.
   Initial values: a(2)=0, a(3)=1. */

#include <stdint.h>
#include <stdbool.h>

#define MAX_K 70                      /* enough for indices up to ~10 000 */

static idx_t fibs[MAX_K];
static idx_t lucas[MAX_K];
static bool init = false;

/* Initialise Fibonacci and Lucas arrays once. */
static void init_fibs(void)
{
    if (init) return;
    fibs[0] = 0;
    fibs[1] = 1;
    for (idx_t i = 2; i < MAX_K; ++i)
        fibs[i] = fibs[i-1] + fibs[i-2];

    lucas[0] = 2;
    lucas[1] = 1;
    for (idx_t i = 2; i < MAX_K; ++i)
        lucas[i] = lucas[i-1] + lucas[i-2];

    init = true;
}

/* a(n) for the OEIS sequence A356998 */
seq_t a(idx_t n)
{
    /* offset 2: undefined for n < 2 */
    if (n < 2)  return 0;
    if (n == 2) return 0;
    if (n == 3) return 1;

    init_fibs();

    for (idx_t k = 2; k < MAX_K-1; ++k) {
        /* 1. Trough: a(4*F(k)+j) = F(k+1), 0 ≤ j ≤ jmax */
        if (k >= 2) {
            idx_t low = 4 * fibs[k];
            idx_t jmax;
            if (k == 2)       jmax = 1;
            else if (k == 3)  jmax = 0;
            else              jmax = fibs[k-3];
            idx_t high = low + jmax;
            if (n >= low && n <= high)
                return (seq_t)fibs[k+1];
        }

        /* 2. Peak: a(L(k)) = 2*F(k-1) */
        if (k >= 3 && n == lucas[k])
            return (seq_t)(2 * fibs[k-1]);

        /* 3. Descent: a(L(k)+j) = 2*F(k-1) – j, 0 ≤ j ≤ F(k-3) */
        if (k >= 4) {
            idx_t low  = lucas[k];
            idx_t high = low + fibs[k-3];
            if (n >= low && n <= high) {
                idx_t val = (2 * fibs[k-1]) - (n - low);
                return (seq_t)val;
            }
        }

        /* 4. Ascent: a(F(k)+j) = F(k-2) + j, 0 ≤ j ≤ F(k-2) */
        if (k >= 5) {
            idx_t low  = fibs[k];
            idx_t high = low + fibs[k-2];
            if (n >= low && n <= high) {
                idx_t val = n - fibs[k-1];   /* since F(k)+F(k-1)=F(k+1)  */
                return (seq_t)val;
            }
        }
    }

    /* If no case matched (should not happen for valid n), return 0. */
    return 0;
}
