/*
 * A333405  Number of integers in base n having exactly three distinct digits
 *          such that the number formed by the consecutive subsequence of the
 *          initial j digits is divisible by j for all j in {1,2,3}.
 * https://oeis.org/A333405
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   Count of 3‑digit base‑n numbers with exactly three distinct
 *            digits such that the first two digits form a number divisible by
 *            2 and the first three digits form a number divisible by 3
 * Range:     u8 n <= 12, u16 n <= 74, u32 n <= 2955, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n < 4) return 0;   /* need at least three distinct digits */

    seq_t freq[6], freq1[6];
    /* freq[r] = number of digits d in [0, n-1] with d % 6 == r
       freq1[r] = number of leading digits (d in [1, n-1]) with d % 6 == r */
    for (int r = 0; r < 6; ++r) {
        if ((idx_t)r < n) {
            freq[r] = ((n - 1 - (idx_t)r) / 6) + 1;
        } else {
            freq[r] = 0;
        }
        freq1[r] = freq[r];
        if (r == 0) {
            if (n > 0) freq1[0] = freq[0] - 1;   /* exclude zero from leading position */
            else freq1[0] = 0;
        }
    }

    seq_t total = 0;
    idx_t n2 = n % 2;
    idx_t n3 = n % 3;
    seq_t a3 = (n3 * n3) % 3;   /* n^2 mod 3 */

    for (int r1 = 0; r1 < 6; ++r1) {
        if (freq1[r1] == 0) continue;
        seq_t s1 = freq1[r1];

        for (int r2 = 0; r2 < 6; ++r2) {
            if (freq[r2] == 0) continue;
            if (n2 == 0) {                /* n even: r2 must be even */
                if ((r2 & 1) != 0) continue;
            } else {                       /* n odd: r1 parity == r2 parity */
                if ((r1 & 1) != (r2 & 1)) continue;
            }
            seq_t s2 = freq[r2];

            for (int r3 = 0; r3 < 6; ++r3) {
                if (freq[r3] == 0) continue;
                seq_t s3 = freq[r3];

                /* modulus 3 condition: (a3*r1 + n3*r2 + r3) % 3 == 0 */
                if (((a3 * (r1 % 3)) + (n3 * (r2 % 3)) + (r3 % 3)) % 3 != 0)
                    continue;

                seq_t add = 0;
                if (r1 == r2 && r1 == r3) {                 /* all residues equal */
                    if (s2 >= 3)
                        add = s1 * (s2 - 1) * (s2 - 2);
                } else if (r1 == r2) {                      /* first two equal */
                    add = s1 * (s2 - 1) * s3;
                } else if (r1 == r3) {                      /* first and third equal */
                    add = s1 * (s3 - 1) * s2;
                } else if (r2 == r3) {                      /* last two equal */
                    add = s1 * s2 * (s3 - 1);
                } else {                                    /* all distinct */
                    add = s1 * s2 * s3;
                }
                total += add;
            }
        }
    }

    return total;
}
