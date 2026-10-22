/*
 * A384234  Irregular triangle read by rows: T(n,k) is the index of the k-th
 *          odd noncomposite divisor in the list of divisors of n, with n >=1,
 *          k >= 1.
 * https://oeis.org/A384234
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf, easy
 * Formula:   not stated
 * Range:     u8 n <= 102, u16 n <= 102, u32 n <= 102, u64 n <= 102,
 *            u128 n <= 102, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A384234 (Irregular triangle read by rows)
 * by generating rows of odd noncomposite divisor indices
 * and returning the n‑th term of the row‑wise concatenation. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Comparator for qsort on uint64_t */
static int cmp_uint64(const void *a, const void *b) {
    uint64_t x = *(const uint64_t *)a;
    uint64_t y = *(const uint64_t *)b;
    if (x < y) return -1;
    if (x > y) return 1;
    return 0;
}

/* Trial division primality test for uint64_t values */
static bool is_prime_uint64(uint64_t x) {
    if (x < 2) return false;
    if (x == 2) return true;
    if ((x & 1ULL) == 0) return false;
    for (uint64_t d = 3; d * d <= x; d += 2) {
        if (x % d == 0) return false;
    }
    return true;
}

/* Recursively generate all divisors of a number given its prime factorisation. */
static void generate_divs(const uint64_t *primes, const int *exp,
                          size_t nf, size_t idx, uint64_t cur,
                          uint64_t *arr, size_t *pos) {
    if (idx == nf) {
        arr[*pos] = cur;
        (*pos)++;
        return;
    }
    uint64_t mult = 1;
    for (int e = 0; e <= exp[idx]; ++e) {
        generate_divs(primes, exp, nf, idx + 1, cur * mult, arr, pos);
        mult *= primes[idx];
    }
}

/* Main OEIS function */
seq_t a(idx_t n) {
    /* ------------------------------------------------------------------
     *  Caching of cumulative term counts per row so repeated calls in
     *  arbitrary order remain efficient.  The cache is grown as needed.
     * ------------------------------------------------------------------ */
    static idx_t *cum = NULL;          /* cumulative term counts per row */
    static size_t cum_len = 0;          /* number of rows cached */
    static size_t cum_cap = 0;          /* allocated capacity */

    if (cum == NULL) {
        cum_cap = 32;
        cum = (idx_t*)malloc(cum_cap * sizeof(idx_t));
        if (!cum) return (seq_t)0;      /* allocation failure */
    }

    /* Build rows until we have at least n terms */
    idx_t total = (cum_len == 0) ? 0 : cum[cum_len - 1];
    while (total < n) {
        size_t r = cum_len + 1;          /* next row number */

        /* Count odd non‑composite divisors of r:
           1 plus the distinct odd prime factors of r.        */
        uint64_t tmp = r;
        size_t odd_prime_cnt = 0;
        for (uint64_t p = 2; p * p <= tmp; ++p) {
            if (tmp % p == 0) {
                if ((p & 1ULL) == 1) odd_prime_cnt++;
                while (tmp % p == 0) tmp /= p;
            }
        }
        if (tmp > 1 && (tmp & 1ULL) == 1) odd_prime_cnt++;

        idx_t row_len = 1 + odd_prime_cnt;
        total += row_len;

        if (cum_len >= cum_cap) {
            cum_cap *= 2;
            idx_t *tmpc = (idx_t*)realloc(cum, cum_cap * sizeof(idx_t));
            if (!tmpc) return (seq_t)0;
            cum = tmpc;
        }
        cum[cum_len++] = total;
    }

    /* Binary search to locate the row containing the n‑th term */
    size_t lo = 0, hi = cum_len - 1, idxRow = 0;
    while (lo <= hi) {
        size_t mid = (lo + hi) >> 1;
        if (cum[mid] >= n) {
            idxRow = mid;
            if (mid == 0) break;
            hi = mid - 1;
        } else {
            lo = mid + 1;
        }
    }

    size_t r = idxRow + 1;                          /* actual row number */
    idx_t prev_cum = (idxRow == 0) ? 0 : cum[idxRow - 1];
    idx_t k = n - prev_cum - 1;                     /* target within row */

    /* Factor r to generate all divisors */
    uint64_t primes[32];
    int exp[32];
    size_t nf = 0;
    uint64_t tmp = r;
    for (uint64_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            primes[nf] = p;
            int e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                e++;
            }
            exp[nf] = e;
            nf++;
        }
    }
    if (tmp > 1) {
        primes[nf] = tmp;
        exp[nf] = 1;
        nf++;
    }

    /* Compute total number of divisors */
    size_t total_div = 1;
    for (size_t i = 0; i < nf; ++i) total_div *= (size_t)(exp[i] + 1);

    /* Generate all divisors into a fixed array (max < 256 for n <= 10000) */
    uint64_t divs[256] = {0};
    size_t pos = 0;
    generate_divs(primes, exp, nf, 0, 1ULL, divs, &pos);

    /* Sort divisors */
    qsort(divs, total_div, sizeof(uint64_t), cmp_uint64);

    /* Find the k‑th odd non‑composite divisor and return its index (1‑based) */
    idx_t counter = 0;
    for (size_t i = 0; i < total_div; ++i) {
        uint64_t d = divs[i];
        if ((d & 1ULL) && (d == 1 || is_prime_uint64(d))) {
            if (counter == k) {
                return (seq_t)(i + 1);      /* index in the full divisor list */
            }
            counter++;
        }
    }

    /* Should never reach here */
    return (seq_t)0;
}
