/*
 * A329469  Perfectly cyclic numbers: numbers k such that the iterations of
 *          the mapping x -> f(x) = x^2 + c (mod k), starting at x = f(c), is
 *          purely periodic for all 0 <= c <= k.
 * https://oeis.org/A329469
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = numbers of the form 2^a·3^b·5^c with 0 ≤ a ≤ 3, 0 ≤ b,c ≤
 *            2, sorted ascending (offset 1)
 * Range:     u8 n <= 7, u16 n <= 36, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    enum { TOTAL = 36 };
    static seq_t table[TOTAL];
    static bool ready = false;

    if (!ready) {
        size_t idx = 0;
        for (idx_t a = 0; a <= 3; ++a) {
            seq_t pow2a = 1;
            for (idx_t i = 0; i < a; ++i)
                pow2a *= (seq_t)2;

            for (idx_t b = 0; b <= 2; ++b) {
                seq_t pow3b = 1;
                for (idx_t i = 0; i < b; ++i)
                    pow3b *= (seq_t)3;

                for (idx_t c = 0; c <= 2; ++c) {
                    seq_t pow5c = 1;
                    for (idx_t i = 0; i < c; ++i)
                        pow5c *= (seq_t)5;

                    table[idx++] = pow2a * pow3b * pow5c;
                }
            }
        }

        /* Simple insertion sort; 36 elements – extremely fast */
        for (size_t i = 1; i < TOTAL; ++i) {
            seq_t key = table[i];
            size_t j = i;
            while (j > 0 && table[j - 1] > key) {
                table[j] = table[j - 1];
                --j;
            }
            table[j] = key;
        }

        ready = true;
    }

    if (n < 1)           /* undefined indices – return 0 safely */
        return (seq_t)0;
    size_t pos = (size_t)(n - 1);
    if (pos >= (size_t)TOTAL)
        return (seq_t)0; /* sequence has only 36 terms */

    return table[pos];
}
