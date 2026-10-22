/*
 * A396152  a(n) is the number of possible outcomes having a run of 3 or more
 *          sixes when a die is rolled n times.
 * https://oeis.org/A396152
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6^n - d(n) where d(n)=5*(d(n-1)+d(n-2)+d(n-3)) This
 *            yields the linear recurrence a(0)=0, a(1)=0, a(2)=0, a(3)=1,
 *            a(n) = 11*a(n-1) - 25*a(n-2) - 25*a(n-3) - 30*a(n-4) (n≥4) The
 *            implementation uses 64‑bit temporaries when seq_t is not larger
 *            than unsigned long long to avoid overflow on 32‑bi...
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 14, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */

seq_t a(idx_t n)
{
    /* initial values a(0)=0, a(1)=0, a(2)=0, a(3)=1 */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)0;
    if (n == 3) return (seq_t)1;

    /* variables holding the last four terms */
    seq_t a0 = (seq_t)0;  /* a(n-4) */
    seq_t a1 = (seq_t)0;  /* a(n-3) */
    seq_t a2 = (seq_t)0;  /* a(n-2) */
    seq_t a3 = (seq_t)1;  /* a(n-1) */

    /* choose algorithm depending on the width of seq_t */
    if (sizeof(seq_t) <= sizeof(unsigned long long)) {
        /* use 64‑bit temporaries for the multiplications */
        unsigned long long u0 = (unsigned long long)a0;
        unsigned long long u1 = (unsigned long long)a1;
        unsigned long long u2 = (unsigned long long)a2;
        unsigned long long u3 = (unsigned long long)a3;
        unsigned long long next;
        for (idx_t i = 4; i <= n; ++i) {
            next = 11ULL * u3
                 - 25ULL * u2
                 - 25ULL * u1
                 - 30ULL * u0;
            u0 = u1;
            u1 = u2;
            u2 = u3;
            u3 = next;
        }
        return (seq_t)u3;
    } else {
        /* seq_t is large enough (e.g., arbitrary‑precision) */
        seq_t next;
        for (idx_t i = 4; i <= n; ++i) {
            next = 11 * a3
                 - 25 * a2
                 - 25 * a1
                 - 30 * a0;
            a0 = a1;
            a1 = a2;
            a2 = a3;
            a3 = next;
        }
        return a3;
    }
}
