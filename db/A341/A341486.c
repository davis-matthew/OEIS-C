/*
 * A341486  Number of ways to write n as an ordered sum of 9 nonprime numbers.
 * https://oeis.org/A341486
 *
 * Offset:    9    (a(9) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 36, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute OEIS sequence A341486:
   Number of ordered sums of 9 nonprime numbers.
   a(n) = number of 9‑tuples (x1,…,x9) with xi ∉ {primes}
          and x1+…+x9 = n.  For n<9, result is 0.  */
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* ------------ Utility: modular arithmetic for NTT ----------------- */
static uint32_t mod_pow_uint32(uint32_t a, uint32_t e, uint32_t mod) {
    uint64_t res = 1, base = a;
    while (e) {
        if (e & 1) res = (res * base) % mod;
        base = (base * base) % mod;
        e >>= 1;
    }
    return (uint32_t)res;
}

static void ntt(uint32_t * a, int n, uint32_t mod, uint32_t root, int invert) {
    /* bit-reversal permutation */
    for (int i = 1, j = 0; i < n; ++i) {
        int bit = n >> 1;
        for (; j & bit; bit >>= 1) j ^= bit;
        j ^= bit;
        if (i < j) { uint32_t tmp = a[i]; a[i] = a[j]; a[j] = tmp; }
    }
    for (int len = 2; len <= n; len <<= 1) {
        uint32_t wlen = mod_pow_uint32(root, (mod - 1) / len, mod);
        if (invert) wlen = mod_pow_uint32(wlen, mod - 2, mod);
        for (int i = 0; i < n; i += len) {
            uint32_t w = 1;
            for (int j = 0; j < len / 2; ++j) {
                uint32_t u = a[i + j];
                uint32_t v = (uint32_t)((uint64_t)a[i + j + len / 2] * w % mod);
                a[i + j] = u + v < mod ? u + v : u + v - mod;
                a[i + j + len / 2] = u >= v ? u - v : u + mod - v;
                w = (uint32_t)((uint64_t)w * wlen % mod);
            }
        }
    }
    if (invert) {
        uint32_t inv_n = mod_pow_uint32(n, mod - 2, mod);
        for (int i = 0; i < n; ++i)
            a[i] = (uint32_t)((uint64_t)a[i] * inv_n % mod);
    }
}

/* ------------ convolution modulo a prime -------------------------- */
static uint32_t* conv_mod_uint32(const uint32_t *a_in, const uint32_t *b_in,
                                 uint32_t mod, int len, int maxN) {
    uint32_t *a = (uint32_t*)malloc(len * sizeof(uint32_t));
    uint32_t *b = (uint32_t*)malloc(len * sizeof(uint32_t));
    memcpy(a, a_in, (maxN + 1) * sizeof(uint32_t));
    for (int i = 0; i < len - maxN - 1; ++i) a[maxN + 1 + i] = 0;
    memcpy(b, b_in, (maxN + 1) * sizeof(uint32_t));
    for (int i = 0; i < len - maxN - 1; ++i) b[maxN + 1 + i] = 0;

    ntt(a, len, mod, 3, 0);
    ntt(b, len, mod, 3, 0);
    for (int i = 0; i < len; ++i)
        a[i] = (uint32_t)((uint64_t)a[i] * b[i] % mod);
    ntt(a, len, mod, 3, 1);

    uint32_t *res = (uint32_t*)malloc((maxN + 1) * sizeof(uint32_t));
    memcpy(res, a, (maxN + 1) * sizeof(uint32_t));

    free(a);
    free(b);
    return res;
}

/* ------------ Garner reconstruction (uint128) --------------------- */
static uint32_t modinv_uint32(uint32_t a, uint32_t mod) {
    int64_t t = 0, newt = 1;
    int64_t r = mod, newr = a;
    while (newr != 0) {
        int64_t q = r / newr;
        int64_t tmp = newt; newt = t - q * newt; t = tmp;
        tmp = newr; newr = r - q * newr; r = tmp;
    }
    if (t < 0) t += mod;
    return (uint32_t)t;
}

/* ------------ Compute sequence up to maxN ------------------------- */
static void compute_up_to(idx_t maxN, seq_t **cache_ptr, idx_t *cached_size_ptr) {
    if (maxN < 9) {
        if (*cached_size_ptr < 10) {
            *cached_size_ptr = 10;
#ifdef __cplusplus
            delete[] *cache_ptr;
            *cache_ptr = new seq_t[*cached_size_ptr];
#else
            free(*cache_ptr);
            *cache_ptr = (seq_t*)malloc(*cached_size_ptr * sizeof(seq_t));
#endif
            for (idx_t i = 0; i < *cached_size_ptr; ++i) (*cache_ptr)[i] = 0;
        }
        return;
    }

    /* Sieve for primes up to maxN */
    bool* is_prime = (bool*)calloc(maxN + 1, sizeof(bool));
    if (!is_prime) return;
    for (idx_t i = 0; i <= maxN; ++i) is_prime[i] = true;
    if (maxN >= 0) is_prime[0] = false;
    if (maxN >= 1) is_prime[1] = false;
    for (idx_t p = 2; p * p <= maxN; ++p) {
        if (is_prime[p]) {
            for (idx_t j = p * p; j <= maxN; j += p) is_prime[j] = false;
        }
    }

    /* Allowed nonprime numbers (include 1) */
    uint32_t *allowed = (uint32_t*)calloc(maxN + 1, sizeof(uint32_t));
    if (!allowed) { free(is_prime); return; }
    for (idx_t i = 1; i <= maxN; ++i) allowed[i] = !is_prime[i] ? 1 : 0;

    free(is_prime);

    /* NTT length (next power of 2 >= 2*maxN+1) */
    int len = 1;
    while (len <= 2 * maxN) len <<= 1;

    /* Moduli (NTT-friendly) and their roots */
    const uint32_t mods[4] = {167772161u, 469762049u, 1224736769u, 1004535809u};
    /* For all these moduli, primitive root 3 is suitable */

    uint32_t *rem[4];
    for (int m = 0; m < 4; ++m)
        rem[m] = (uint32_t*)calloc(maxN + 1, sizeof(uint32_t));

    /* For each modulus, compute G^9 */
    for (int m = 0; m < 4; ++m) {
        uint32_t *poly = (uint32_t*)malloc((maxN + 1) * sizeof(uint32_t));
        memcpy(poly, allowed, (maxN + 1) * sizeof(uint32_t));
        for (int iter = 1; iter < 9; ++iter) {
            uint32_t *next = conv_mod_uint32(poly, allowed, mods[m], len, maxN);
            free(poly);
            poly = next;
        }
        memcpy(rem[m], poly, (maxN + 1) * sizeof(uint32_t));
        free(poly);
    }

    /* Reconstruct integer values via Garner */
    unsigned __int128 *big = (unsigned __int128*)calloc((maxN + 1), sizeof(unsigned __int128));
    for (idx_t n = 0; n <= maxN; ++n) {
        if (n < 9) { big[n] = 0; continue; }
        unsigned __int128 x = 0, m_prod = 1;
        for (int i = 0; i < 4; ++i) {
            uint32_t mi = mods[i];
            uint32_t re = rem[i][n];
            uint32_t m_mod = (uint32_t)(m_prod % mi);
            uint32_t inv = modinv_uint32(m_mod, mi);
            uint32_t t = re;
            uint32_t xm = (uint32_t)(x % mi);
            if (t >= xm) t -= xm;
            else t += mi - xm;
            t = (uint32_t)((uint64_t)t * inv % mi);
            x += m_prod * t;
            m_prod *= mi;
        }
        big[n] = x;
    }

    /* Allocate new cache */
    idx_t needed = maxN + 1;
    seq_t *new_cache;
#ifdef __cplusplus
    delete[] *cache_ptr;
    new_cache = new seq_t[needed];
#else
    free(*cache_ptr);
    new_cache = (seq_t*)malloc(needed * sizeof(seq_t));
#endif
    for (idx_t n = 0; n <= maxN; ++n)
        new_cache[n] = (seq_t)big[n];

    /* Free temporary structures */
    free(allowed);
    free(big);
    for (int m = 0; m < 4; ++m) free(rem[m]);

    /* Store result */
    *cache_ptr = new_cache;
    *cached_size_ptr = needed;
}

/* ------------ Public sequence function ----------------------------- */
seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cached_size = 0;

    if (n < 9) return 0;

    if (n + 1 > cached_size) {
        compute_up_to((idx_t)n, &cache, &cached_size);
    }
    return cache[n];
}
