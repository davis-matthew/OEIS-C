/*
 * A333836  Number of ways to write n as the difference of two positive
 *          k-gonal numbers for k >= 3.
 * https://oeis.org/A333836
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5000, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Counts the number of ways to write n as the difference of two
   positive k‑gonal numbers for k >= 3.  The sequence A333836 satisfies
   a(n) = A333822(n) - A177025(n) for n > 2, but the implementation
   below uses a direct enumeration of solutions. */

#include <stddef.h>
#include <stdint.h>

/* helper: compute all positive divisors of n (n <= 20000 when used)
   and store them in buf, updating cnt.  The array buf must be large
   enough to hold all divisors (here 256 is more than sufficient). */
static void get_divisors(idx_t n, idx_t *buf, size_t *cnt)
{
    *cnt = 0;
    for (idx_t i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            buf[(*cnt)++] = i;
            if (i != n / i) {
                buf[(*cnt)++] = n / i;
            }
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* offset is 1, but safe */
    seq_t count = 0;
    idx_t two_n = n * 2;

    /* divisors of 2n: candidate differences d = a-b */
    idx_t d_buf[256];
    size_t d_cnt;
    get_divisors(two_n, d_buf, &d_cnt);

    /* temp buffers for divisors of |val-2| */
    idx_t t_buf[256];
    size_t t_cnt;

    for (size_t i = 0; i < d_cnt; ++i) {
        idx_t d = d_buf[i];
        idx_t val = two_n / d;           /* val = 2n/d */
        long long diff_signed = (long long)val - 2LL;   /* val-2 may be negative */

        if (diff_signed == 0) continue;  /* s would be 1, impossible because s>d */

        idx_t abs_diff = diff_signed >= 0 ? (idx_t)diff_signed
                                          : (idx_t)(-diff_signed);

        get_divisors(abs_diff, t_buf, &t_cnt);

        for (size_t j = 0; j < t_cnt; ++j) {
            idx_t t = t_buf[j];           /* t = k-2 */
            idx_t k = t + 2;              /* k >= 3 */

            long long s_ll = diff_signed / (long long)t + 1LL; /* s = (val-2)/t + 1 */
            if (s_ll <= 0) continue;      /* s must be positive */

            idx_t s = (idx_t)s_ll;

            /* parity condition: d and s must have same parity */
            if ((s & 1) != (d & 1)) continue;

            /* require a > b, i.e., s > d */
            if (s <= d) continue;

            /* all conditions satisfied: one valid representation */
            count++;
        }
    }

    return count;
}
