/*
 * A358840  Primorial base exp-function reduced modulo 6.
 * https://oeis.org/A358840
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A276086(n) mod 6 – primorial base exp‑function
 * Range:     u8 n <= 9845, u16 n <= 9845, u32 n <= 9845, u64 n <= 9845,
 *            u128 n <= 9845, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* First primes – enough for all n that fit comfortably in 64 bits */
static const idx_t primes[] = {
    2, 3, 5, 7, 11, 13, 17, 19, 23, 29,
    31, 37, 41, 43, 47, 53, 59, 61, 67, 71,
    73, 79, 83, 89, 97, 101, 103, 107, 109, 113,
    127, 131, 137, 139, 149, 151, 157, 163, 167, 173,
    179, 181, 191, 193, 197, 199, 211, 223, 227, 229,
    233, 239, 241, 251, 257, 263, 269, 271, 277, 281,
    283, 293, 307, 311, 313, 317, 331, 337, 347, 349
};
#define PRIME_COUNT (sizeof(primes)/sizeof(primes[0]))

seq_t a(idx_t n)
{
    /* n == 0 => 1 by definition of A276086(0) */
    if (n == 0)
        return (seq_t)1;

    /* Build primorial weights w[i] = product of primes[0 .. i-1] (w[0] = 1) */
    idx_t w[PRIME_COUNT];
    size_t k = 0;
    w[0] = 1;
    while (k + 1 < PRIME_COUNT) {
        if (w[k] > n / primes[k])               /* would overflow or exceed n */
            break;
        w[k + 1] = w[k] * primes[k];
        ++k;
    }
    /* Ensure k indexes the largest weight <= n */
    while (k > 0 && w[k] > n)
        --k;

    /* Decode the primorial digits, accumulating exponents */
    seq_t e2 = 0, e3 = 0;
    int parity5 = 0;            /* parity of odd exponents of primes ≡ 5 (mod 6) */
    idx_t rem = n;

    for (int i = (int)k; i >= 0; --i) {
        idx_t digit = rem / w[i];
        rem       %= w[i];
        idx_t p    = primes[i];

        if (p == 2)
            e2 = (seq_t)digit;
        else if (p == 3)
            e3 = (seq_t)digit;
        else if (p % 6 == 5)
            if (digit & 1)
                parity5 ^= 1;
    }

    /* Compute a(n) = (2‑factor × 3‑factor × 5‑factor) mod 6 */
    seq_t res = 1;

    if (e2 == 0)
        res = 1;
    else
        res = (e2 & 1) ? (seq_t)2 : (seq_t)4;            /* 2 or 4 mod 6 */

    if (e3 > 0)
        res = (res * (seq_t)3) % (seq_t)6;               /* multiply by 3 if present */

    if (parity5)
        res = (res * (seq_t)5) % (seq_t)6;               /* multiply by 5 if needed */

    return res;
}
