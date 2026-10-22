/*
 * A332759  Number of involutions (plus identity) in a fixed Sylow 2-subgroup
 *          of the symmetric group of degree n.
 * https://oeis.org/A332759
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product(A332757(k)) where k ranges over the positions of
 *            1 bits in the binary expansion of n. A332757 is defined by
 *            A332757(0)=1 and A332757(k)=A332757(k-1)^2 + 2^(2^(k-1)-1)
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

static seq_t f[64];          /* A332757(k) values */
static int fmax = -1;        /* largest k computed so far */

/* Ensure that f[0 .. max_k] are computed.  f[0] = 1, and for k > 0
   f[k] = f[k-1]^2 + 2^(2^(k-1)-1).  The exponent 2^(k-1)-1 fits in
   idx_t for all k <= 63. */
static void compute_f_up_to(int max_k)
{
    if (max_k < 0)
        return;
    while (fmax < max_k) {
        ++fmax;
        if (fmax == 0) {
            f[0] = (seq_t)1;
        } else {
            /* compute 2^(2^(fmax-1)-1) */
            idx_t shift_exp = (1ULL << (fmax - 1)) - 1;   /* 2^(fmax-1)-1 */
            seq_t add = pow2(shift_exp);
            f[fmax] = f[fmax - 1] * f[fmax - 1] + add;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0)            /* empty product */
        return (seq_t)1;

    /* Find the highest set bit position to know how much of f[] to compute. */
    idx_t temp = n;
    int max_bit = 0;
    while (temp) {
        ++max_bit;
        temp >>= 1;
    }
    compute_f_up_to(max_bit - 1);

    seq_t result = (seq_t)1;
    temp   = n;
    int pos = 0;
    while (temp) {
        if (temp & 1U)
            result *= f[pos];
        temp >>= 1;
        ++pos;
    }
    return result;
}
