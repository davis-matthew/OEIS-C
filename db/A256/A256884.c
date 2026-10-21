/*
 * A256884  Numbers divisible by prime(d+1) for each digit d of their base-4
 *          representation.
 * https://oeis.org/A256884
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n-th nonnegative integer such that for every digit d of
 *            its base‑4 representation, the integer is divisible by
 *            prime(d+1). (prime(1)=2, prime(2)=3, prime(3)=5, prime(4)=7)
 * Range:     u8 n <= 17, u16 n <= 926, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* Euclidean algorithm for 64‑bit unsigned integers (used only for primes ≤ 7). */
static inline uint64_t ugcd(uint64_t a, uint64_t b)
{
    while (b) {
        uint64_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Check whether a number satisfies the digit‑divisibility condition. */
static inline bool satisfies(seq_t m)
{
    /* Map digit → prime(d+1). */
    static const uint64_t primes[4] = { 2, 3, 5, 7 };
    uint64_t lcm = 1;                /* LCM of the primes required by the digits. */
    seq_t tmp = m;

    /* Special case 0: 0 is divisible by every prime. */
    if (tmp == 0) {
        return true;
    }

    while (tmp > 0) {
        /* Extract one base‑4 digit. */
        uint8_t digit = (uint8_t)(tmp % 4);
        uint64_t prime = primes[digit];

        /* Update LCM with the new prime. */
        uint64_t g = ugcd(lcm, prime);
        lcm = (lcm / g) * prime;

        /* Move to the next digit. */
        tmp = tmp / 4;
    }

    /* The number satisfies the condition iff it is divisible by the computed LCM. */
    return (m % (seq_t)lcm == 0);
}

/* Return the n‑th term of sequence A256884 (1‑based). */
seq_t a(idx_t n)
{
    idx_t count = 0;   /* Count of discovered qualifying numbers. */
    seq_t m = 0;       /* Candidate number, starting at 0. */

    while (1) {
        if (satisfies(m)) {
            ++count;
            if (count == n) {
                return m;
            }
        }
        /* Increment candidate. */
        m = m + 1;
    }
}
