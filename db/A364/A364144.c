/*
 * A364144  Number of distinct representations for n in base 2, using digits
 *          -1,0,1, whose sum of digits is 0.
 * https://oeis.org/A364144
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   Compute the number of distinct representations of n in base 2
 *            using digits -1, 0, 1 whose sum of digits is 0. The algorithm
 *            uses a DP over the binary digits of n, keeping track of the
 *            carry from the addition n + y (where y collects the -1 digits),
 *            and the difference between the number of +1 and -...
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   // for uint64_t, if needed

/* Maximum number of bits processed.  For n up to 10000 this easily covers all
   needed bits; it also works for larger n within 64-bit range. */
#define MAX_BITS 70
/* Offset for the difference array (-MAX_BITS .. +MAX_BITS).  Size = 2*MAX_BITS+1. */
#define MAX_D (2*MAX_BITS+1)
#define D_OFFSET (MAX_BITS)

/* Helper to zero a 2D array of seq_t. */
static void zero_dp(seq_t arr[2][MAX_D]) {
    for (int c = 0; c < 2; ++c)
        for (int d = 0; d < MAX_D; ++d)
            arr[c][d] = 0;
}

seq_t a(idx_t n) {
    /* Determine number of bits needed for n. */
    int bitlen_n = 0;
    idx_t tmp = n;
    while (tmp) {
        ++bitlen_n;
        tmp >>= 1;
    }

    /* Use a fixed number of bits; for n <= 10000 the chosen bound is loose
       enough to capture all solutions. */
    int use_bits = bitlen_n + 35;
    if (use_bits > MAX_BITS) use_bits = MAX_BITS;

    static seq_t dp[2][MAX_D];
    static seq_t nextdp[2][MAX_D];

    zero_dp(dp);
    dp[0][D_OFFSET] = 1;   /* carry = 0, difference = 0 */

    for (int i = 0; i < use_bits; ++i) {
        zero_dp(nextdp);

        int n_bit = (int)((n >> i) & 1);
        for (int carry = 0; carry < 2; ++carry) {
            for (int d = -MAX_BITS; d <= MAX_BITS; ++d) {
                seq_t val = dp[carry][d + D_OFFSET];
                if (!val) continue;

                /* y_bit = 0 */
                {
                    int y_bit = 0;
                    int sum = n_bit + y_bit + carry;
                    int s_bit = sum & 1;
                    int new_carry = sum >> 1;
                    int new_d = d + (y_bit - s_bit);
                    nextdp[new_carry][new_d + D_OFFSET] += val;
                }

                /* y_bit = 1 */
                {
                    int y_bit = 1;
                    int sum = n_bit + y_bit + carry;
                    int s_bit = sum & 1;
                    if (y_bit == 1 && s_bit == 1) continue;   /* x & y = 0 condition */
                    int new_carry = sum >> 1;
                    int new_d = d + (y_bit - s_bit);
                    nextdp[new_carry][new_d + D_OFFSET] += val;
                }
            }
        }

        /* Move to the next bit. */
        for (int c = 0; c < 2; ++c)
            for (int d = 0; d < MAX_D; ++d)
                dp[c][d] = nextdp[c][d];
    }

    return dp[0][D_OFFSET];   /* carry zero and equal numbers of +1 and -1 digits */
}
