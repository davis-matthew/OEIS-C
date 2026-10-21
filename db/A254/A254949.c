/*
 * A254949  Number of length 2 1..(n+1) arrays with every leading partial sum
 *          divisible by 2, 3, 5, 7 or 11.
 * https://oeis.org/A254949
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ordered pairs (x,y) with 1 ≤ x,y ≤ n+1, x
 *            divisible by any of the primes {2,3,5,7,11} and (x+y) divisible
 *            by any of those primes
 * Range:     u8 n <= 19, u16 n <= 210, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const uint64_t primes[5] = {2,3,5,7,11};

seq_t a(idx_t n)
{
    /* m = n+1 */
    idx_t m = n + 1U;

    /* precompute lcm (product) for all non‑empty subsets of primes
       and their parity sign (+1 for odd size, -1 for even size). */
    uint64_t lcm[32];
    int    sign[32];
    for (int s = 1; s < 32; ++s) {
        uint64_t prod = 1ULL;
        int cnt = 0;
        for (int k = 0; k < 5; ++k) {
            if (s & (1 << k)) {
                prod *= primes[k];
                ++cnt;
            }
        }
        lcm[s] = prod;
        sign[s] = (cnt & 1) ? 1 : -1;
    }

    seq_t result = 0;

    /* iterate over all possible first elements x */
    for (idx_t x = 1; x <= m; ++x) {
        /* check if x is divisible by any of the primes */
        int good_x = 0;
        for (int k = 0; k < 5; ++k) {
            if (x % primes[k] == 0U) {
                good_x = 1;
                break;
            }
        }
        if (!good_x) continue;

        /* inclusion–exclusion to count y satisfying the second condition */
        seq_t pos = 0;   /* sum over odd-sized subsets */
        seq_t neg = 0;   /* sum over even-sized subsets */

        for (int s = 1; s < 32; ++s) {
            uint64_t mod = lcm[s];
            uint64_t rem = (mod - (x % mod)) % mod;   /* (-x) mod mod */

            seq_t cnt_y;
            if (rem == 0ULL) {
                cnt_y = (seq_t)(m / mod);
            } else if (rem > m) {
                cnt_y = 0;
            } else {
                cnt_y = (seq_t)(( (m - rem) / mod ) + 1);
            }

            if (sign[s] > 0)
                pos += cnt_y;
            else
                neg += cnt_y;
        }
        seq_t y_contrib = pos - neg;
        result += y_contrib;
    }

    return result;
}
