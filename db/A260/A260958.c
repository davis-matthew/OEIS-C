/*
 * A260958  Expansion of (a(q) - 3*a(q^2) + 3*a(q^3) - 4*a(q^4) + 3*a(q^6)) /
 *          6 in powers of q where a() is a cubic AGM theta function.
 * https://oeis.org/A260958
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = EulerTransformPeriod36(b)[n] with b(n) = * [-3, 1, 1, -1,
 *            -3, -4, -3, -1, 0, 1, * -3, -2, -3, 1, 1, -1, -3, -2, -3, -1, *
 *            1, 1, -3, -2, -3, 1, 0, -1, -3, -4, * -3, -1, 1, 1, -3, -2]
 *            (period 36) * * The Euler transform is defined recursively: * *
 *            C(m) = Sum_{d|m} d * b(d) * A(0) = 1 * A(m)...
 * Range:     i8 n <= 20, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <assert.h>

/* Periodic base sequence b(n) used in the Euler transform */
static const int period[36] = {
   -3,  1,  1, -1, -3, -4, -3, -1,  0,  1,
   -3, -2, -3,  1,  1, -1, -3, -2, -3, -1,
    1,  1, -3, -2, -3,  1,  0, -1, -3, -4,
   -3, -1,  1,  1, -3, -2
};

/* Array capacity: enough for all queries up to about 10 000   */
#define MAX_CAPACITY 20001

static seq_t A_seq[MAX_CAPACITY];
static seq_t C_seq[MAX_CAPACITY];
static idx_t A_sz = 0;          /* number of computed A terms */

/* Ensure that A_seq and C_seq are computed up to index 'max' (inclusive). */
static void ensure_upto(idx_t max)
{
    if (max < A_sz)
        return;                      /* already computed */

    /* Safety check: do not exceed the fixed buffer. */
    assert(max < MAX_CAPACITY - 1);

    /* Initialise new entries to zero. */
    for (idx_t i = A_sz; i <= max; ++i) {
        A_seq[i] = 0;
        C_seq[i] = 0;
    }

    /* Base case: A(0) = 1.  This is required for the recurrence. */
    if (A_sz == 0) {
        A_seq[0] = (seq_t)1;
        A_sz    = 1;
    }

    /* Compute A and C for indices ≥ A_sz up to max. */
    for (idx_t m = A_sz; m <= max; ++m) {
        /* Compute C(m) = Σ_{d|m} d * b(d) */
        seq_t csum = 0;
        for (idx_t d = 1; d <= m / d; ++d) {
            if (m % d == 0) {
                idx_t d2 = m / d;
                int bd   = period[(int)((d - 1) % 36)];
                csum += (seq_t)d * (seq_t)bd;
                if (d2 != d) {
                    int bd2 = period[(int)((d2 - 1) % 36)];
                    csum += (seq_t)d2 * (seq_t)bd2;
                }
            }
        }
        C_seq[m] = csum;

        /* Compute A(m) = (1/m) * Σ_{k=1..m} C(k) * A(m-k) */
        seq_t sum = 0;
        for (idx_t k = 1; k <= m; ++k)
            sum += C_seq[k] * A_seq[m - k];
        A_seq[m] = sum / (seq_t)m;
    }

    A_sz = max + 1;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;              /* Offset 0 */

    ensure_upto(n - 1);               /* Ensure A(n-1) is ready */
    return A_seq[n - 1];              /* a(n) = A(n-1) */
}
