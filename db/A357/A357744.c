/*
 * A357744  a(n) is the least k such that prime(n) * k occurs in one of the
 *          eight main spokes of a square spiral with 1 in the center.
 * https://oeis.org/A357744
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 6548, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime, min_multiplier_for_two
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute sequence A357744: a(n) = minimal k > 0 such that
   prime(n)*k lies on one of the eight main spokes of the square spiral.
   The spiral positions on the spokes are given by
     (2r‑1)² + k*r   for k = 1,2,…,8 (east, northeast, north, northwest,
     west, southwest, south, southeast) and r ≥ 1.
   For each prime p we solve the quadratic congruence
     4r² + (k‑4)r + 1 ≡ 0 (mod p)     (1)
   using Tonelli–Shanks to obtain the smallest r that yields a valid
   multiple.  The corresponding multiplier k = ((2r‑1)² + k*r)/p is
   computed and the minimal such k over all directions is returned. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/get_prime.h"
#include "../utils/min_multiplier_for_two.h"

/* ---------- modular arithmetic helpers ---------- */

static inline uint64_t mod_pow(uint64_t a, uint64_t e, uint64_t mod)
{
    uint64_t res = 1;
    a %= mod;
    while (e) {
        if (e & 1) res = (res * a) % mod;
        a = (a * a) % mod;
        e >>= 1;
    }
    return res;
}

static inline uint64_t mod_inv(uint64_t a, uint64_t p)
{
    /* p is prime, a != 0 mod p */
    return mod_pow(a, p - 2, p);
}

/* Tonelli–Shanks algorithm: compute sqrt(n) mod p, p odd prime.
   Returns (root, found) where found is true iff a square root exists. */
static bool tonelli_shanks(uint64_t n, uint64_t p, uint64_t *root)
{
    if (n == 0) { *root = 0; return true; }
    if (p == 2) { *root = n & 1; return true; }

    /* Legendre symbol: n^((p-1)/2) mod p == 1 means residue */
    if (mod_pow(n, (p - 1) >> 1, p) != 1) return false;

    if (p % 4 == 3) {
        *root = mod_pow(n, (p + 1) >> 2, p);
        return true;
    }

    /* factor p-1 as q*2^s with q odd */
    uint64_t q = p - 1;
    uint64_t s = 0;
    while ((q & 1) == 0) {
        q >>= 1;
        ++s;
    }

    /* find a quadratic non-residue z */
    uint64_t z = 2;
    while (mod_pow(z, (p - 1) >> 1, p) != p - 1) ++z;

    uint64_t c = mod_pow(z, q, p);
    uint64_t r = mod_pow(n, (q + 1) >> 1, p);
    uint64_t t = mod_pow(n, q, p);
    uint64_t m = s;

    while (t != 1) {
        uint64_t i = 1;
        uint64_t t2i = (t * t) % p; /* t^(2) */
        while (t2i != 1) {
            t2i = (t2i * t2i) % p;
            ++i;
        }
        uint64_t b = mod_pow(c, 1ULL << (m - i - 1), p);
        r = (r * b) % p;
        c = (b * b) % p;
        t = (t * c) % p;
        m = i;
    }
    *root = r;
    return true;
}

/* Return minimal multiplier for given prime p (>=3) */
static uint64_t min_multiplier(uint64_t p)
{
    uint64_t best = (uint64_t)-1;     /* large sentinel */

    const int dirs[8] = {1,2,3,4,5,6,7,8};
    uint64_t inv8 = mod_inv(8, p);

    for (int idx = 0; idx < 8; ++idx) {
        uint64_t k = dirs[idx];                /* direction coefficient */
        int64_t b_signed = (int64_t)k - 4;     /* could be negative */
        uint64_t b = (uint64_t)((b_signed % (int64_t)p + (int64_t)p) % (int64_t)p);
        /* compute discriminant Δ = b² - 16 (mod p) */
        uint64_t bb = (b * b) % p;
        uint64_t six = 16 % p;
        uint64_t delta = (bb + p - six) % p;

        uint64_t sqrt_delta;
        bool exist;
        if (!tonelli_shanks(delta, p, &sqrt_delta))
            continue;            /* no solution for this direction */

        /* two possible roots: -b ± sqrt_delta  (all modulo p) */
        uint64_t r_candidates[2];
        uint64_t negb = (p - b) % p;
        r_candidates[0] = ((negb + sqrt_delta) % p) * inv8 % p;
        r_candidates[1] = ((negb + (p - sqrt_delta)) % p) * inv8 % p;

        for (int c = 0; c < 2; ++c) {
            uint64_t r = r_candidates[c];
            if (r == 0) r = p;                      /* use r = p if root 0 */
            if (r < 1) continue;                   /* skip zero root */

            uint64_t two_r_minus1 = 2 * r - 1;
            uint64_t n = two_r_minus1 * two_r_minus1 + k * r;  /* ≡ 0 mod p */
            uint64_t m = n / p;                     /* candidate multiplier */

            if (m < best) best = m;
        }
    }
    return best;
}

/* ---------- main sequence function ---------- */

seq_t a(idx_t n)
{
    if (n < 1) return 0;    /* undefined for n < 1 */

    /* Retrieve the n-th prime (prime(1)=2) */
    uint64_t p = get_prime(n);

    uint64_t m;
    if (p == 2) {
        m = min_multiplier_for_two();
    } else {
        m = min_multiplier(p);   /* minimal k such that p*k on a spoke */
    }

    return (seq_t)m;
}
