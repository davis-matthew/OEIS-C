/*
 * A346154  a(n) is the least prime of the form n^k+n+1 with k>1, or 0 if
 *          there is no such prime.
 * https://oeis.org/A346154
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least prime of n^k + n + 1 with k > 1, 0 if none. a(1) =
 *            3 (special case), a(n) = 0 when n ≡ 1 (mod 3) and n > 1
 * Range:     u8 n <= 8, u16 n <= 22, u32 n <= 22, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   big_pow, is_prime, is_prime_big, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"          /* is_prime(idx_t) for 64‑bit seq_t   */
#include "../utils/is_prime_big.h"     /* is_prime_big(seq_t) for big builds*/
#include "../utils/big_pow.h"          /* big_pow(seq_t, seq_t)              */
#include "../utils/seq_mul.h"          /* seq_mul(seq_t, seq_t) (big builds) */

seq_t a(idx_t n)
{
    /* Special cases */
    if (n == 1) return 3;           /* 1^2 + 1 + 1 = 3 */
    if ((n % 3) == 1) return 0;     /* Polynomial always divisible by 3 */

    /* n > 1 and n mod 3 != 1 => we search for the smallest prime */
    /* Different strategy for 64‑bit seq_t and arbitrary‑precision builds */
    if (sizeof(seq_t) == sizeof(uint64_t)) {
        /* 64‑bit case: use unsigned __int128 for intermediate values */
        unsigned __int128 pow128 = (unsigned __int128)n * n;   /* n^2  (k = 2) */
        while (true) {
            unsigned __int128 candidate = pow128 + n + 1;
            if (candidate > (unsigned __int128)~(uint64_t)0) {
                /* Exceeds the capacity of seq_t: no more candidates */
                break;
            }
            if (is_prime((idx_t)candidate)) {
                return (seq_t)candidate;
            }
            /* Advance to the next k */
            pow128 *= n;
            if (pow128 > (unsigned __int128)~(uint64_t)0) break;
        }
        return 0;
    } else {
        /* Arbitrary‑precision seq_t: use big_pow and is_prime_big.
           We construct the power incrementally to keep the cost low. */
        seq_t n_seq = (seq_t)n;
        seq_t power = seq_mul(n_seq, n_seq);   /* n^2, k = 2            */
        while (true) {
            seq_t candidate = power + n_seq + 1;
            if (is_prime_big(candidate)) {
                return candidate;
            }
            power = seq_mul(power, n_seq);     /* multiply by n to get next power */
        }
        /* Unreachable, but keeps the compiler happy */
        return 0;
    }
}
