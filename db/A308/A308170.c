/*
 * A308170  Start with the number 7, repeatedly square every digit in place to
 *          get a new number; in the limit this process converges (reading
 *          from right to left) to the string shown here.
 * https://oeis.org/A308170
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = digit n of the fixed point of the morphism d →
 *            reverse(d²) (beginning with digit 7). This sequence starts
 *            1,6,3,9,1,8,1,4,6,1,6,1,6,3,1,6,3,1,6,3,9,1,6,3,…
 * Range:     u8 n <= 9562, u16 n <= 9562, u32 n <= 9562, u64 n <= 9562,
 *            u128 n <= 9562, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* mappings for digits 0–9
   mapping[d] = string representing reverse(d²) */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

static const char * const MAP[10] = {
    "0",  /* 0² = 0   → 0          */
    "1",  /* 1² = 1   → 1          */
    "4",  /* 2² = 4   → 4          */
    "9",  /* 3² = 9   → 9          */
    "61", /* 4² = 16  → 61         */
    "52", /* 5² = 25  → 52         */
    "63", /* 6² = 36  → 63         */
    "94", /* 7² = 49  → 94         */
    "46", /* 8² = 64  → 46         */
    "18"  /* 9² = 81  → 18         */
};

static const unsigned char MAPLEN[10] = {
    1,1,1,1,2,2,2,2,2,2
};

/* pre‑computed word lengths: len[i][d] = length of f^i(d)
   where f(d) = MAP[d] (the morphism).   */
#define MAX_ITER 63   /* sufficient for all n up to ~10⁴   */
static uint64_t len[MAX_ITER + 1][10];
static bool table_ok = false;

/* initialise the length table once */
static void init_table(void)
{
    for (int d = 0; d < 10; d++)
        len[0][d] = 1;            /* f⁰(d) is just d itself   */

    for (int i = 1; i <= MAX_ITER; i++) {
        for (int d = 0; d < 10; d++) {
            uint64_t sum = 0;
            const char *rep = MAP[d];
            for (int k = 0; rep[k] != '\0'; k++) {
                int c = rep[k] - '0';
                sum += len[i - 1][c];
            }
            len[i][d] = sum;
        }
    }
    table_ok = true;
}

/* recursively find the digit at position pos (1‑based) in
   f^iter(d).  iter ≥ 0.  */
static int digit_at(int d, int iter, uint64_t pos)
{
    if (iter == 0)
        return d;                 /* base word of one digit */
    const char *rep = MAP[d];
    for (int k = 0; rep[k] != '\0'; k++) {
        int c = rep[k] - '0';
        uint64_t l = len[iter - 1][c];
        if (pos <= l)
            return digit_at(c, iter - 1, pos);
        pos -= l;
    }
    return -1;                    /* should never happen   */
}

seq_t a(idx_t n)
{
    if (n == 0)                /* undefined in OEIS – 0 or 1 */
        return (seq_t)0;

    if (!table_ok)
        init_table();

    /* use a sufficiently large fixed iteration count; the word
       has converged for all positions up to the limits expected
       in the tests (n ≤ ~10⁴). */
    int iter = MAX_ITER;
    int digit = digit_at(7, iter, (uint64_t)n);
    return (seq_t)digit;
}
