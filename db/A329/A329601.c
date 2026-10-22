/*
 * A329601  The squarefree kernel of Product prime(e(i)), when n = Product
 *          prime(i)^e(i).
 * https://oeis.org/A329601
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product of the primes indexed by the distinct exponents
 *            in the prime factorisation of n. That is, if n = ∏ p_i^{e_i},
 *            then a(n) = ∏_{e in {e_i | e_i > 0}} prime(e) (square‑free
 *            version)
 * Range:     u8 n <= 9867, u16 n <= 9867, u32 n <= 9867, u64 n <= 9867,
 *            u128 n <= 9867, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* First 64 primes (prime(1)=2, prime(2)=3, ..., prime(64)=311) */
    static const uint64_t primes[64] = {
        2ull, 3ull, 5ull, 7ull, 11ull, 13ull, 17ull, 19ull, 23ull, 29ull,
        31ull, 37ull, 41ull, 43ull, 47ull, 53ull, 59ull, 61ull, 67ull, 71ull,
        73ull, 79ull, 83ull, 89ull, 97ull, 101ull, 103ull, 107ull, 109ull, 113ull,
        127ull, 131ull, 137ull, 139ull, 149ull, 151ull, 157ull, 163ull, 167ull, 173ull,
        179ull, 181ull, 191ull, 193ull, 197ull, 199ull, 211ull, 223ull, 227ull, 229ull,
        233ull, 239ull, 241ull, 251ull, 257ull, 263ull, 269ull, 271ull, 277ull, 281ull,
        283ull, 293ull, 307ull, 311ull
    };

    uint64_t temp = (uint64_t)n;          /* work with native integer */
    bool seen[65] = {0};                  /* seen[e] = true if exponent e already used */
    seq_t result = (seq_t)1;              /* product of selected primes */

    /* Trial division: factorise temp */
    for (uint64_t i = 2; i * i <= temp; ) {
        uint64_t e = 0;
        while (temp % i == 0) {
            temp /= i;
            ++e;
        }
        if (e > 0 && !seen[e]) {
            seen[e] = true;
            result *= (seq_t)primes[e - 1];   /* prime(e) is primes[e-1] */
        }
        i += (i == 2 ? 1 : 2);   /* after 2, test only odd divisors */
    }
    if (temp > 1 && !seen[1]) {         /* leftover prime factor, exponent 1 */
        seen[1] = true;
        result *= (seq_t)primes[0];     /* prime(1) = 2 */
    }

    return result;
}
