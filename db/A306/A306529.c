/*
 * A306529  x-value of the smallest solution to x^2 - p*y^2 =
 *          2*(-1)^((p+1)/4), p = A002145(n).
 * https://oeis.org/A306529
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = minimal positive integer x such that x^2 − p·y^2 =
 *            2·(−1)^((p+1)/4), where p is the n‑th prime ≡ 3 mod 4
 * Range:     u8 n <= 9, u16 n <= 23, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   build_primes_upto, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"
#include "../utils/floor_sqrt.h"

/* Cached list of primes ≡ 3 (mod 4) and memoised sequence values */
static idx_t *prime3_cache = NULL;
static size_t prime3_cnt   = 0;
static seq_t *a_cache      = NULL;
static bool   init_done    = false;

/* Initialise primes ≥ 3 mod 4 up to a fixed bound (sufficient for n≈10000) */
static void init_primes(void)
{
    if (init_done) return;

    const idx_t LIMIT = 2500000;          /* enough to cover n ≈ 10000 */
    uint64_t *pr = NULL;
    size_t pc   = 0;

    build_primes_upto(LIMIT, &pr, &pc);

    /* Count how many of those primes are ≡ 3 mod 4 */
    size_t cnt = 0;
    for (size_t i = 0; i < pc; ++i)
        if (pr[i] % 4 == 3)
            ++cnt;

    prime3_cache = (idx_t*)malloc(cnt * sizeof(idx_t));
    size_t pos = 0;
    for (size_t i = 0; i < pc; ++i)
        if (pr[i] % 4 == 3)
            prime3_cache[pos++] = (idx_t)pr[i];

    prime3_cnt = cnt;
    free(pr);

    a_cache = (seq_t*)calloc(prime3_cnt, sizeof(seq_t));
    init_done = true;
}

/* Compute the minimal x for a given prime p ≡ 3 (mod 4) */
static seq_t minimal_x_for_prime(idx_t p)
{
    /* Desired RHS: +2 if ((p+1)/4) even, otherwise -2 */
    bool pos_sign = (((p + 1) / 4) & 1) == 0;

    /* Continued fraction of √p */
    const idx_t a0 = (idx_t)floor_sqrt((seq_t)p);
    idx_t m = 0;
    idx_t d = 1;
    idx_t a = a0;

    /* Convergents: h_{-2}=0, h_{-1}=1; k_{-2}=1, k_{-1}=0 */
    seq_t h_nm2 = 0, h_nm1 = 1;
    seq_t k_nm2 = 1, k_nm1 = 0;

    for (;;) {
        seq_t h = (seq_t)a * h_nm1 + h_nm2;
        seq_t k = (seq_t)a * k_nm1 + k_nm2;

        seq_t lhs = h * h;
        seq_t rhs = ((seq_t)p) * k * k;

        seq_t diff;
        bool positive;
        if (lhs >= rhs) {
            diff = lhs - rhs;
            positive = true;
        } else {
            diff = rhs - lhs;
            positive = false;
        }

        if (diff == (seq_t)2 &&
            ((positive && pos_sign) || (!positive && !pos_sign)))
            return h;

        /* Advance convergents */
        h_nm2 = h_nm1; h_nm1 = h;
        k_nm2 = k_nm1; k_nm1 = k;

        /* Next partial quotient of the continued fraction */
        idx_t m_next = d * a - m;
        idx_t d_next = (p - m_next * m_next) / d;
        idx_t a_next = (a0 + m_next) / d_next;

        m = m_next;
        d = d_next;
        a = a_next;
    }
}

/* Requested sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n = 0 */

    init_primes();
    if ((size_t)n > prime3_cnt) return 0;  /* out of pre‑computed range */

    size_t idx = (size_t)(n - 1);
    if (a_cache[idx] == 0)
        a_cache[idx] = minimal_x_for_prime(prime3_cache[idx]);

    return a_cache[idx];
}
