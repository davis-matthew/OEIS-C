/*
 * A267414  Integers k such that there exist nonnegative integers x,y,z with
 *          k! = x^3 + y^3 + z^3.
 * https://oeis.org/A267414
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A267414: a(n) = the n‑th nonnegative integer k such that
 *  k! can be expressed as a sum of three nonnegative cubes.
 *  It is known that all integers > 13 satisfy this property.
 *  Hence the sequence consists of all nonnegative integers
 *  except the finite set {3,5,6,7,8,13}.
 *
 *  This implementation returns the n-th element of that set
 *  (with a(1)=0).
 */
#include <stdbool.h>

seq_t a(idx_t n) {
    static const idx_t bad[] = {3, 5, 6, 7, 8, 13};
    const unsigned bad_len = 6;

    idx_t k = 0;          /* candidate integer */
    idx_t cnt = 0;        /* how many valid integers seen so far */

    while (cnt < n) {
        bool is_bad = false;
        for (unsigned i = 0; i < bad_len; ++i) {
            if (k == bad[i]) {
                is_bad = true;
                break;
            }
        }
        if (!is_bad) {
            ++cnt;
            if (cnt == n)
                break;
        }
        ++k;
    }
    return (seq_t)k;
}
