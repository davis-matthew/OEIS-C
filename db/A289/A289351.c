/*
 * A289351  Starting from one digit move right by x steps, x being the value
 *          of the digit. If the steps go beyond the least significant digits
 *          they continue from the left side. Then repeat the process from the
 *          reached digit. The sequence lists the numbers such that all the
 *          digits are touched just one time an...
 * https://oeis.org/A289351
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 48, u16 n <= 713, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Brute‑force search for OEIS A289351: numbers with no zero digits in which the
   permutation f(i) = (i + d[i]) mod k (k = number of digits) is a single cycle.
   a(0) = 0, a(n) for n ≥ 1 is the n‑th smallest such number. */

#include <stdbool.h>
#include <stdint.h>

#define MAX_DIGITS 40   /* covers up to 39 decimal digits (128‑bit numbers) */

seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)0;

    idx_t count = 0;
    idx_t val   = 1;               /* start searching from 1 */

    while (1) {
        /* Skip numbers containing the digit 0 */
        idx_t tmp = val;
        bool has0 = false;
        while (tmp) {
            if (tmp % 10 == 0) { has0 = true; break; }
            tmp /= 10;
        }
        if (!has0) {
            /* Extract digits most‑significant first */
            int digits[MAX_DIGITS];
            int k = 0;
            tmp = val;
            while (tmp) {
                digits[k++] = tmp % 10;
                tmp /= 10;
            }
            for (int i = 0; i < k / 2; ++i) {
                int d = digits[i];
                digits[i] = digits[k - 1 - i];
                digits[k - 1 - i] = d;
            }

            /* Build the permutation f(i) and check bijectivity */
            bool used[MAX_DIGITS] = {0};
            int f[MAX_DIGITS];
            bool ok = true;

            for (int i = 0; i < k; ++i) {
                int step = (i + digits[i]) % k;
                if (used[step]) { ok = false; break; }
                used[step] = true;
                f[i] = step;
            }

            if (ok) {
                /* Check that f is a single k‑cycle */
                bool visited[MAX_DIGITS] = {0};
                int cur = 0, cycles = 0;
                do {
                    visited[cur] = true;
                    cur = f[cur];
                    ++cycles;
                } while (cur != 0);

                if (cycles == k) {
                    ++count;
                    if (count == n)
                        return (seq_t)val;
                }
            }
        }
        ++val;                       /* try next integer */
    }
}
