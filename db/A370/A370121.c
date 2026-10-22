/*
 * A370121  Triangle read by rows: T(n,k) = A002110(n) + A002110(k), 0 <= k <=
 *          n; sums of two primorials, not necessarily distinct.
 * https://oeis.org/A370121
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = primorial(r) + primorial(c) * where r and c are derived
 *            from the triangular indexing * of a: r =
 *            floor((sqrt(8*n+1)-1)/2), c = n - r*(r+1)/2
 * Range:     u8 n <= 13, u16 n <= 27, u32 n <= 54, u64 n <= 135,
 *            u128 n <= 376, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

#ifdef __cplusplus
#include <vector>
#endif

/* ---------- C++ build (with arbitrary‑precision seq_t) ---------- */
#ifdef __cplusplus
static std::vector<uint64_t> primes;
static std::vector<seq_t> primorials;

/* Enters primes until at least 'needed' primes are available */
static void ensure_primes(size_t needed)
{
    while (primes.size() < needed) {
        uint64_t candidate = primes.empty() ? 2 : primes.back() + 1;
        bool ok = false;
        while (!ok) {
            ok = true;
            uint64_t limit = static_cast<uint64_t>(sqrt(static_cast<double>(candidate))) + 1;
            for (size_t j = 0; j < primes.size(); ++j) {
                uint64_t p = primes[j];
                if (p > limit) break;
                if (candidate % p == 0) { ok = false; break; }
            }
            if (!ok) ++candidate;
        }
        primes.push_back(candidate);
    }
}

/* Enters primorials until at least index 'needed' is present */
static void ensure_primorials(size_t needed)
{
    if (primorials.empty()) {
        primorials.push_back(1);  /* primorial(0) = 1 */
    }
    while (primorials.size() <= needed) {
        size_t i = primorials.size();          /* new index */
        uint64_t prime = primes[i-1];
        primorials.push_back(primorials.back() * (seq_t)prime);
    }
}
#else
/* ---------- C build (seq_t is an unsigned integer) ---------- */
static uint64_t *primes = NULL;
static size_t primes_cnt = 0;
static seq_t *primorials = NULL;
static size_t primorials_cnt = 0;

/* Enters primes until at least 'needed' primes are available */
static void ensure_primes(size_t needed)
{
    while (primes_cnt < needed) {
        uint64_t candidate = (primes_cnt == 0) ? 2 : primes[primes_cnt-1] + 1;
        bool ok = false;
        while (!ok) {
            ok = true;
            uint64_t limit = (uint64_t)sqrt((double)candidate) + 1;
            for (size_t j = 0; j < primes_cnt; ++j) {
                uint64_t p = primes[j];
                if (p > limit) break;
                if (candidate % p == 0) { ok = false; break; }
            }
            if (!ok) ++candidate;
        }
        uint64_t *tmp = (uint64_t*)realloc(primes, (primes_cnt + 1) * sizeof(uint64_t));
        if (!tmp) abort();
        primes = tmp;
        primes[primes_cnt++] = candidate;
    }
}

/* Enters primorials until at least index 'needed' is present */
static void ensure_primorials(size_t needed)
{
    if (primorials_cnt == 0) {
        seq_t *tmp = (seq_t*)malloc(sizeof(seq_t));
        if (!tmp) abort();
        tmp[0] = (seq_t)1;          /* primorial(0) = 1 */
        primorials = tmp;
        primorials_cnt = 1;
    }
    while (primorials_cnt <= needed) {
        uint64_t prime = primes[primorials_cnt-1];
        seq_t new_val = primorials[primorials_cnt-1] * (seq_t)prime;
        seq_t *tmp = (seq_t*)realloc(primorials, (primorials_cnt + 1) * sizeof(seq_t));
        if (!tmp) abort();
        primorials = tmp;
        primorials[primorials_cnt++] = new_val;
    }
}
#endif

/* ---------------- main sequence function ---------------- */
seq_t a(idx_t n)
{
    /* compute row index r and column index c from triangular indexing */
    /* r satisfies r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    double tmp = sqrt(8.0 * (double)n + 1.0);
    idx_t r = (idx_t)((tmp - 1.0) / 2.0);
    /* adjust in case of rounding errors */
    while ((r + 1) * (r + 2) / 2 <= n) ++r;
    while (r * (r + 1) / 2 > n) --r;
    idx_t start = r * (r + 1) / 2;
    idx_t c = n - start;  /* 0 <= c <= r */

    /* ensure enough primes and primorials are pre‑computed */
    ensure_primes((size_t)r);
    ensure_primorials((size_t)r);

    /* answer */
    return primorials[r] + primorials[c];
}
