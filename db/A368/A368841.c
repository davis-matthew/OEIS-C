/*
 * A368841  Nonnegative integers whose binary expansions (without leading
 *          zeros) have no three equally spaced equal digits.
 * https://oeis.org/A368841
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy, fini, full
 * Formula:   Sequence of nonnegative integers whose binary expansions contain
 *            * no three equally spaced equal digits (OEIS A368841). * The
 *            sequence is finite: the longest admissible binary expansion has
 *            * at most 8 digits (Van der Waerden number W(2,3)=9), so all
 *            admissible * numbers are in the range [0,255]....
 * Range:     u8 n <= 40, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    static seq_t list[256];      /* maximum needed size */
    static size_t count = 0;     /* number of admissible values found */
    static bool init_done = false;

    if (!init_done) {
        for (idx_t val = 0; val < 256; ++val) {
            uint64_t v = (uint64_t)val;
            /* compute bit length */
            unsigned len;
            if (v == 0)
                len = 1;
            else {
                len = 0;
                uint64_t tmp = v;
                while (tmp) {
                    ++len;
                    tmp >>= 1;
                }
            }

            bool ok = true;
            for (unsigned i = 0; i + 2 < len && ok; ++i) {
                uint64_t b0 = (v >> i) & 1ULL;
                for (unsigned d = 1; i + 2 * d < len; ++d) {
                    uint64_t b1 = (v >> (i + d)) & 1ULL;
                    if (b1 != b0) continue;
                    uint64_t b2 = (v >> (i + 2 * d)) & 1ULL;
                    if (b2 == b0) {
                        ok = false;
                        break;
                    }
                }
            }
            if (ok)
                list[count++] = (seq_t)val;
        }
        init_done = true;
    }

    if (n >= 1 && (idx_t)n <= count)
        return list[n - 1];
    else
        return (seq_t)0;   /* out of bounds: return 0 (safe for tests) */
}
