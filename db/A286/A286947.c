/*
 * A286947  Triangle read by rows in which row(n) = {T(n, k)} is the
 *          lexicographically earliest list of n numbers such that adding 1 to
 *          some T(n, k) gives a row of numbers each divisible by prime(k).
 * https://oeis.org/A286947
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) where r is the row index such that r*(r+1)/2 >= n,
 *            k = n - r*(r-1)/2 is the position in that row, and T(r,k) =
 *            (primorial(r) / prime(k)) * m_k, with m_k = ((prime(k) - 1) *
 *            inv_mod(primorial(r)/prime(k) mod prime(k), prime(k))) mod
 *            prime(k)
 * Range:     u8 n <= 10, u16 n <= 21, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* Simple modular exponentiation for uint64_t */
static uint64_t powmod64(uint64_t a, uint64_t e, uint64_t mod) {
    uint64_t r = 1 % mod;
    a %= mod;
    while (e) {
        if (e & 1) r = (r * a) % mod;
        a = (a * a) % mod;
        e >>= 1;
    }
    return r;
}

/* Dynamic array of primes (1‑based indexing in logical sense) */
static uint32_t *prime_list = NULL;
static size_t prime_list_cnt = 0;
static size_t prime_list_cap = 0;

/* Ensure that at least 'need' primes are generated */
static void ensure_primes_up_to(size_t need) {
    if (prime_list_cnt >= need) return;

    /* Grow capacity if necessary */
    if (prime_list_cap < need) {
        size_t new_cap = prime_list_cap ? prime_list_cap * 2 : 64;
        while (new_cap < need) new_cap *= 2;
        uint32_t *tmp = (uint32_t *)realloc(prime_list, new_cap * sizeof(uint32_t));
        if (!tmp) exit(EXIT_FAILURE);
        prime_list = tmp;
        prime_list_cap = new_cap;
    }

    uint32_t candidate = (prime_list_cnt == 0) ? 2 : prime_list[prime_list_cnt - 1] + 1;
    while (prime_list_cnt < need) {
        /* trivial primality test using already found primes */
        bool is_prime = true;
        for (size_t i = 0; i < prime_list_cnt; ++i) {
            uint32_t p = prime_list[i];
            if ((uint64_t)p * (uint64_t)p > (uint64_t)candidate) break;
            if (candidate % p == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime) {
            prime_list[prime_list_cnt++] = candidate;
        }
        ++candidate;
    }
}

/* Dynamic array of primorials: primorial_cache[n] = product of first n primes (primorial(0)=1) */
static seq_t *primorial_cache = NULL;
static size_t primorial_cache_len = 0;   /* number of elements stored (incl. 0) */
static size_t primorial_cache_cap = 0;

static void ensure_primorial_cache_len(size_t target) {
    if (primorial_cache_len > target) return;

    if (primorial_cache_len == 0) {
        primorial_cache_cap = 16;
        primorial_cache = (seq_t *)malloc(primorial_cache_cap * sizeof(seq_t));
        if (!primorial_cache) exit(EXIT_FAILURE);
        primorial_cache[0] = (seq_t)1;
        primorial_cache_len = 1;
    }

    if (primorial_cache_cap <= target) {
        size_t new_cap = primorial_cache_cap ? primorial_cache_cap * 2 : 64;
        while (new_cap <= target) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(primorial_cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        primorial_cache = tmp;
        primorial_cache_cap = new_cap;
    }

    for (size_t i = primorial_cache_len; i <= target; ++i) {
        seq_t p = (seq_t)prime_list[i - 1]; /* prime_list is 0‑based */
        primorial_cache[i] = primorial_cache[i - 1] * p;
    }
    primorial_cache_len = target + 1;
}

/* Main function to compute a(n) */
seq_t a(idx_t n) {
    if (n == 0) { /* offset 1, but guard against zero if called */
        return (seq_t)0;
    }

    /* Find row 'r' such that triangular(r-1) < n <= triangular(r) */
    idx_t r = 0;
    while ((r * (r + 1)) / 2 < n) ++r;
    idx_t prev_tri = (r * (r - 1)) / 2;
    idx_t k = n - prev_tri; /* 1‑based position within row r */

    /* Ensure primes and primorials up to r exist */
    ensure_primes_up_to((size_t)r);
    ensure_primorial_cache_len((size_t)r);

    seq_t primorial_r = primorial_cache[(size_t)r];
    uint32_t pk = prime_list[(size_t)k - 1]; /* prime(k) */

    /* Compute a_mod = product_{i!=k} prime(i) mod pk */
    uint64_t a_mod = 1;
    for (size_t i = 0; i < (size_t)r; ++i) {
        if ((size_t)i + 1 == (size_t)k) continue; /* skip prime(k) */
        a_mod = (a_mod * (prime_list[i] % pk)) % pk;
    }

    /* Compute modular inverse of a_mod modulo pk */
    uint64_t inv_a = powmod64(a_mod, (uint64_t)pk - 2, pk);
    uint64_t m_k = ((uint64_t)pk - 1) * inv_a % pk;
    if (m_k == 0) m_k = pk; /* ensure positive, though this case cannot occur */

    /* Compute T(r,k) = (primorial_r / pk) * m_k */
    seq_t prod_except = primorial_r / (seq_t)pk;
    seq_t result = prod_except * (seq_t)m_k;
    return result;
}
