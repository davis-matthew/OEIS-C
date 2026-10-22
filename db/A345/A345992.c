/*
 * A345992  Let m = A344005(n) = smallest m such that n divides m*(m+1); a(n)
 *          = gcd(n,m).
 * https://oeis.org/A345992
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(n, m) where m is the smallest positive m * such that
 *            n divides m*(m+1). The function finds m by * solving a system of
 *            congruences for each prime power of n * and chooses the minimal
 *            solution. Then it returns a(n)
 * Range:     u8 n <= 269, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/gcd.h"

static void factor_uint64(uint64_t n, uint64_t *mods, uint64_t *residues,
                          size_t *cnt) {
    /* fills mods with each prime power (p^e) dividing n,
     * residues with 0 for assigning m≡0 mod p^e and
     * (p^e - 1) for m≡-1 mod p^e.  mod count is *cnt. */
    size_t c = 0;
    for (uint64_t p = 2; p * p <= n; ++p) {
        if (n % p == 0) {
            uint64_t pe = 1;
            while (n % p == 0) {
                n /= p;
                pe *= p;
            }
            mods[c] = pe;
            residues[c] = 0; /* placeholder, will be set per assignment later */
            ++c;
        }
    }
    if (n > 1) {         /* remaining prime factor */
        mods[c] = n;
        residues[c] = 0;
        ++c;
    }
    *cnt = c;
}

/* extended gcd: returns g = gcd(a,b) and finds x,y s.t. a*x + b*y = g.
 * We use it only when a and b are coprime, so x is modular inverse of a mod b. */
static int64_t extended_gcd(int64_t a, int64_t b, int64_t *x, int64_t *y) {
    int64_t x0 = 1, y0 = 0, x1 = 0, y1 = 1;
    while (b != 0) {
        int64_t q = a / b;
        int64_t t = a % b;
        a = b;
        b = t;
        t = x0 - q * x1;
        x0 = x1;
        x1 = t;
        t = y0 - q * y1;
        y0 = y1;
        y1 = t;
    }
    *x = x0;
    *y = y0;
    return a;
}

static uint64_t modinv(uint64_t a, uint64_t mod) {
    int64_t x, y;
    extended_gcd((int64_t)a, (int64_t)mod, &x, &y);
    x %= (int64_t)mod;
    if (x < 0) x += mod;
    return (uint64_t)x;
}

/* solve x ≡ a mod mod_part under current solution r mod m,
 * returning new r after merging.  Assumes m and mod_part coprime. */
static uint64_t crt_merge(uint64_t r, uint64_t m, uint64_t a, uint64_t mod_part) {
    uint64_t inv = modinv(m % mod_part, mod_part);
    uint64_t diff = (a + mod_part - (r % mod_part)) % mod_part;
    uint64_t k = (diff * inv) % mod_part;
    return r + m * k; /* will be < m*mod_part = new modulus */
}

seq_t a(idx_t n) {
    if (n == 0) return 0; /* undefined, but safeguard */
    uint64_t nn = (uint64_t)n;

    /* factor nn into prime powers */
    uint64_t mods[16];
    uint64_t residues[16];
    size_t k;
    factor_uint64(nn, mods, residues, &k);

    /* store prime powers in an array; they are mutually coprime */
    uint64_t powermods[16];
    for (size_t i = 0; i < k; ++i) powermods[i] = mods[i];

    uint64_t best_m = nn; /* maximum possible candidate is n */
    size_t total_assign = (k == 0) ? 1 : (1U << k);
    for (size_t mask = 0; mask < total_assign; ++mask) {
        uint64_t r = 0;   /* current solution */
        uint64_t mod = 1; /* current modulus, always divides n */
        for (size_t i = 0; i < k; ++i) {
            uint64_t m_i = powermods[i];
            uint64_t a_i = (mask & (1U << i)) ? (m_i - 1) : 0; /* choose residue */
            r = crt_merge(r, mod, a_i, m_i);
            mod *= m_i; /* mod now equals product of processed moduli */
        }
        uint64_t m_candidate = (r == 0) ? nn : r;
        if (m_candidate < best_m) best_m = m_candidate;
    }
    /* compute gcd(n, m) */
    seq_t result = gcd((seq_t)nn, (seq_t)best_m);
    return result;
}
