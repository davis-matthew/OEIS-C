/*
 * A320829  Continued fraction of the positive constant t in (1,2) such that
 *          the partial denominators form the Beatty sequence {floor((n+1)*t),
 *          n >= 0}.
 * https://oeis.org/A320829
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr
 * Formula:   a(n) = floor((n+1)*t), where t ≈
 *            1.4467466246485661263399614145878451657650505718…
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* t is represented exactly as the rational number p/q with p = 1446746624648566126,
   q = 10^18.  For n up to about 5·10^17 the product (n+1)*p/q is accurate enough to
   recover the correct integer value of floor((n+1)*t). */

#include <stdint.h>

static const unsigned long long T_NUM = 1446746624648566126ULL;
static const unsigned long long T_DEN = 1000000000000000000ULL;

seq_t a(idx_t n)
{
    /* Using uint64_t arithmetic for the index and __int128 for the multiplication
       to avoid overflow. The result fits in seq_t because (n+1)*t is moderate
       for all practical arguments. */
    idx_t k = n + 1u;
    unsigned __int128 prod = (unsigned __int128)k * T_NUM;
    seq_t res = (seq_t)(prod / T_DEN);
    return res;
}
