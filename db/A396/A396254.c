/*
 * A396254  Irregular table read by rows: let p = prime(n) and chi be any
 *          Dirichlet character modulo p of order p-1. Then T(n,k), 0 <= k <=
 *          p-2, is the algebraic degree of tau(chi^k) over Q, where tau is
 *          the Gauss sum.
 * https://oeis.org/A396254
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, look, tabf
 * Formula:   For n ≥ 1 let p = prime(n). Let k be the 0‑based * position of n
 *            within the row for p. Then a(n) = d * φ(d), where * d = (p−1) /
 *            gcd(k, p−1). The sequence is read row‑wise from * the irregular
 *            table
 * Range:     u8 n <= 92, u16 n <= 9172, u32 n <= 9172, u64 n <= 9172,
 *            u128 n <= 9172, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* ---------- static data ----------
 *  primes[]  – first MAX_PRIMES primes (enough for n ≤ 10000)
 *  cum[]     – cumulative row lengths up to each prime
 *  primes_cnt – how many primes were stored
 *  init_done   – flag indicating that tables are ready      */
#define MAX_PRIMES 20000
static uint32_t primes[MAX_PRIMES];
static uint64_t cum[MAX_PRIMES];
static size_t primes_cnt = 0;
static bool init_done    = false;

/* Euclidean gcd for uint64_t (used for k and p−1).  */
static inline uint64_t gcd64(uint64_t a, uint64_t b)
{
    while (b != 0) {
        uint64_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Pre‑compute primes up to LIMIT and cumulative row sizes. */
static void init(void)
{
    const uint32_t LIMIT = 200000;          /* includes primes up to ≈10⁵ */
    bool *is_composite = (bool *)malloc((size_t)(LIMIT + 1) * sizeof(bool));
    if (is_composite == NULL) return;      /* allocation failure → abort */

    size_t cnt = 0;
    for (uint32_t i = 2; i <= LIMIT && cnt < (size_t)MAX_PRIMES; ++i) {
        if (!is_composite[i]) {
            primes[cnt] = i;
            cum[cnt] = (cnt == 0) ? (uint64_t)(i - 1) : cum[cnt - 1] + (uint64_t)(i - 1);
            ++cnt;
            if ((uint64_t)i * i <= LIMIT) {
                for (uint32_t j = i * i; j <= LIMIT; j += i)
                    is_composite[j] = true;
            }
        }
    }
    free(is_composite);
    primes_cnt = cnt;
    init_done = true;
}

/* Euler totient φ(n) for n ≤ primes[MAX_PRIMES] */
static uint64_t phi_uint64(uint64_t n)
{
    if (n <= 1) return 1;   /* φ(1) = 1, φ(0) unused */
    uint64_t result = n;
    uint64_t tmp = n;
    for (size_t i = 0; i < primes_cnt && (uint64_t)primes[i] * primes[i] <= tmp; ++i) {
        uint32_t q = primes[i];
        if (tmp % q == 0) {
            while (tmp % q == 0) tmp /= q;
            result = result / q * (q - 1);
        }
    }
    if (tmp > 1)                /* remaining prime factor > sqrt(original n) */
        result = result / tmp * (tmp - 1);
    return result;
}

/* ----------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;    /* below offset – undefined, return 0 */

    if (!init_done) init();

    /* binary search for the prime row containing n */
    size_t lo = 0, hi = primes_cnt - 1;
    while (lo < hi) {
        size_t mid = lo + (hi - lo) / 2;
        if (cum[mid] >= (uint64_t)n) hi = mid;
        else                         lo = mid + 1;
    }
    size_t prime_index = lo;                    /* row index (0‑based) */
    uint32_t p = primes[prime_index];
    uint64_t prev_len = (prime_index == 0) ? 0ULL : cum[prime_index - 1];

    /* 0‑based offset k within the row */
    uint64_t k = (uint64_t)n - prev_len - 1ULL;

    uint64_t p_minus_1 = (uint64_t)p - 1ULL;
    uint64_t d = p_minus_1 / gcd64(k, p_minus_1);

    uint64_t phi_d = phi_uint64(d);
    uint64_t value = d * phi_d;

    return (seq_t)value;
}
