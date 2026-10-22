/*
 * A351567  The second least significant nonzero digit in the primorial base
 *          expansion of n, or 0 if there is no such digit.
 * https://oeis.org/A351567
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = second least significant nonzero digit in the primorial
 *            base * expansion of n, or 0 if there is no such digit
 * Range:     u8 n <= 9757, u16 n <= 9757, u32 n <= 9757, u64 n <= 9757,
 *            u128 n <= 9757, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute primorial base representation of n.
 * The bases are the successive primes 2,3,5,7,11,...  Each digit d_k satisfies
 * 0 <= d_k < next_prime_k.  The function returns the second nonzero digit
 * (counting from the least significant one).  If fewer than two nonzero
 * digits exist, it returns 0. */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Immediate return for n == 0 (representation is all zeros). */
    if (n == 0) return 0;

    /* Use 64‑bit arithmetic for the intermediate value n. */
    uint64_t rem = (uint64_t)n;

    /* First few primes are enough for any 64‑bit n. */
    static const uint64_t primes[] = {
        2ull, 3ull, 5ull, 7ull, 11ull, 13ull, 17ull, 19ull,
        23ull, 29ull, 31ull, 37ull, 41ull, 43ull, 47ull,
        53ull, 59ull, 61ull, 67ull, 71ull, 73ull, 79ull,
        83ull, 89ull, 97ull
    };
    const size_t prime_count = sizeof(primes)/sizeof(primes[0]);

    seq_t second_digit = 0;   /* The value to return. */
    int   nonzero_seen = 0;   /* How many non‑zero digits have been seen. */

    for (size_t i = 0; i < prime_count && rem > 0; ++i) {
        uint64_t digit = rem % primes[i];
        rem /= primes[i];

        if (digit != 0) {
            ++nonzero_seen;
            if (nonzero_seen == 2) {
                second_digit = (seq_t)digit;
                break;   /* The second nonzero digit has been found. */
            }
        }
    }

    /* If fewer than two nonzero digits exist, second_digit remains 0. */
    return second_digit;
}
