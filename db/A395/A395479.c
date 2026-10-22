/*
 * A395479  Decimal expansion of 1135547732074278753 / 46505268690923380.
 * https://oeis.org/A395479
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   not stated
 * Range:     u8 n <= 106, u16 n <= 106, u32 n <= 106, u64 n <= 106,
 *            u128 n <= 106, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) is the n-th decimal digit (counting from the first digit of the
 * integer part) of the rational number 1135547732074278753 / 46505268690923380.
 * The offset is 2: a(2) = 2, a(3) = 4, a(4) = 4, a(5) = 1, ….
 */
#include <stdint.h>

static const uint64_t NUM = 1135547732074278753ULL;
static const uint64_t DEN = 46505268690923380ULL;

seq_t a(idx_t n)
{
    /* Position in the digit sequence (first digit = position 1). */
    if (n < 2)
        return 0;          /* undefined for n < 2, but return 0 */

    idx_t pos = (idx_t)(n - 1);   /* pos >= 1 */

    /* First two digits from the integer part */
    if (pos == 1) return 2;
    if (pos == 2) return 4;

    /* Compute fractional digits by long division */
    unsigned __int128 rem = (unsigned __int128)(NUM % DEN);

    seq_t digit = 0;
    for (idx_t i = 3; i <= pos; ++i) {
        rem *= 10;
        digit = (seq_t)(rem / DEN);
        rem %= DEN;
    }
    return digit;
}
