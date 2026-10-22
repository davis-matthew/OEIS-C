/*
 * A344005  a(n) = smallest positive m such that n divides the oblong number
 *          m*(m+1).
 * https://oeis.org/A344005
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest positive m such that n divides m*(m+1)
 * Range:     u8 n <= 256, u16 n <= 9998, u32 n <= 9998, u64 n <= 9998,
 *            u128 n <= 9998, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* modular multiplicative inverse of a modulo mod, 0 < a < mod, gcd(a,mod)=1 */
static uint64_t modinv_uint64(uint64_t a, uint64_t mod)
{
    int64_t t = 0, newt = 1;
    int64_t r = (int64_t)mod, newr = (int64_t)(a % mod);
    while (newr != 0) {
        uint64_t q = (uint64_t)(r) / (uint64_t)(newr);
        int64_t tmp = t - (int64_t)q * newt;
        t = newt; newt = tmp;
        tmp = r - (int64_t)q * newr;
        r = newr; newr = tmp;
    }
    if (r > 1) return 0;              /*  a not invertible mod mod (should not happen) */
    if (t < 0) t += (int64_t)mod;
    return (uint64_t)t;
}

/* construct m via CRT given residues and moduli arrays */
static uint64_t crt_solution(const uint64_t *mods, const uint64_t *res, int k)
{
    uint64_t x = 0;    /* current solution modulo M */
    uint64_t M = 1;    /* current modulus */
    for (int i = 0; i < k; ++i) {
        uint64_t q = mods[i];
        uint64_t r = res[i] % q;          /* desired residue modulo q */

        /* solve x ≡ r (mod q) and x ≡ x (mod M) */
        uint64_t a = M % q;
        uint64_t inv = modinv_uint64(a, q);
        uint64_t delta = (r + q - (x % q)) % q;
        uint64_t t = (delta * inv) % q;
        x = x + M * t;                    /* new solution */
        M *= q;                           /* new modulus */
        x %= M;                           /* keep x < M */
    }
    return x;  /* 0 <= x < product of mods ( = n ) */
}

/* --------------------------------------------------------------------- */

seq_t a(idx_t n)
{
    /* handle offset: a(1)=1 */
    if (n == (idx_t)1) return (seq_t)1;

    uint64_t n64 = (uint64_t)n;

    /* factorisation: find distinct primes and their powers */
    uint64_t prime_powers[64];
    int num_factors = 0;
    uint64_t temp = n64;

    /* factor 2 separately */
    if ((temp & 1ULL) == 0) {
        uint64_t pw = 1;
        while ((temp & 1ULL) == 0) {
            temp >>= 1;
            pw <<= 1;
        }
        prime_powers[num_factors++] = pw;
    }

    /* odd factors */
    for (uint64_t p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            uint64_t pw = 1;
            while (temp % p == 0) {
                temp /= p;
                pw *= p;
            }
            prime_powers[num_factors++] = pw;
        }
    }
    if (temp > 1)  /* remaining prime factor */
        prime_powers[num_factors++] = temp;   /* its power is itself */

    /* if n is a prime power, answer is n-1 */
    if (num_factors == 1)
        return (seq_t)(n64 - 1ULL);

    /* iterate over all subsets of choices (0 -> residue 0, 1 -> residue q-1) */
    uint64_t best = (uint64_t)-1;   /* large sentinel */
    uint64_t subsets = 1ULL << num_factors;
    for (uint64_t mask = 0; mask < subsets; ++mask) {
        uint64_t residues[64];
        for (int i = 0; i < num_factors; ++i) {
            uint64_t q = prime_powers[i];
            residues[i] = (mask & (1ULL << i)) ? (q - 1ULL) : 0ULL;
        }
        uint64_t val = crt_solution(prime_powers, residues, num_factors);
        if (val == 0) val = n64;   /* smallest positive is n if CRT gives 0 */
        if (val < best) best = val;
        /* early exit: minimal possible is 1 */
        if (best == 1ULL) break;
    }
    return (seq_t)best;
}
