/*
 * A396006  Numbers having a representation as the quotient of two fibbinary
 *          numbers (A003714).
 * https://oeis.org/A396006
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   sequence of integers having a representation as the quotient *
 *            of two fibbinary numbers (A003714). The terms are listed in
 *            increasing * order and a(1)=1
 * Range:     u8 none, u16 none, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <string.h>

/* The maximum value to be examined when building the sequence.
 * It must be large enough to contain the first 10 000 terms;
 * 200 000 gives far more than required. */
static const size_t MAX_VAL = 200000;

/* Cached sequence values – the nth term is seq_vals[n-1] */
static seq_t *seq_vals   = NULL;
static size_t seq_len    = 0;
static int   initialized = 0;

/* Build the sequence once */
static void compute_sequence(void)
{
    /* Build list of fibbinary numbers up to MAX_VAL */
    size_t fib_cap = 2048;
    seq_t  *fib   = (seq_t*)malloc(fib_cap * sizeof(seq_t));
    size_t  fib_cnt = 0;

    for (idx_t i = 1; i <= MAX_VAL; ++i) {
        seq_t v = (seq_t)i;
        if ((v & (v >> 1)) == 0) {               /* no consecutive ones */
            if (fib_cnt == fib_cap) {
                fib_cap <<= 1;
                fib = (seq_t*)realloc(fib, fib_cap * sizeof(seq_t));
            }
            fib[fib_cnt++] = v;
        }
    }

    /* Array marking which numbers are representable */
    unsigned char *present = (unsigned char*)calloc(MAX_VAL + 1, sizeof(unsigned char));

    /* For every pair (a,b) of fibbinary numbers (a>=b) test if a%b==0 */
    for (size_t ii = 0; ii < fib_cnt; ++ii) {
        seq_t a = fib[ii];
        for (size_t jj = 0; jj <= ii; ++jj) {
            seq_t b = fib[jj];
            seq_t m = a % b;
            if (m == 0) {
                seq_t n = a / b;
                if (n <= MAX_VAL)
                    present[(size_t)n] = 1;
            }
        }
    }

    /* Count how many numbers are present */
    seq_len = 0;
    for (size_t i = 1; i <= MAX_VAL; ++i)
        if (present[i]) seq_len++;

    /* Allocate and fill the sequence array */
    seq_vals = (seq_t*)malloc(seq_len * sizeof(seq_t));
    size_t k = 0;
    for (size_t i = 1; i <= MAX_VAL; ++i)
        if (present[i]) seq_vals[k++] = (seq_t)i;

    /* Clean up */
    free(fib);
    free(present);
}

/* Public function required by the exercise */
seq_t a(idx_t n)
{
    if (!initialized) {
        compute_sequence();
        initialized = 1;
    }
    if (n == 0 || (size_t)n > seq_len)
        return 0;   /* undefined for out‑of‑range requests */
    return seq_vals[(size_t)n - 1];
}
