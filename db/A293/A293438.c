/*
 * A293438  Product of deficiencies of proper divisors of n.
 * https://oeis.org/A293438
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Product_{d|n, d<n} ( 2*d - sigma(d) )
 * Range:     i8 n <= 44, i16 n <= 104, i32 n <= 489, i64 n <= 1759,
 *            i128 n <= 7479, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* sigma(d) = sum of all divisors of d (including d itself)   */

#include <stdint.h>
#include <stddef.h>

/* ---------- Primitive integer sqrt for uint64_t ---------- */
static uint64_t int_sqrt_u64(uint64_t n)
{
    uint64_t low = 0, high = n, mid, res = 0;
    while (low <= high) {
        mid = (low + high) >> 1;
        uint64_t sq = mid * mid;
        if (sq <= n) {
            res = mid;
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return res;
}

/* ---------- Small sieve to generate primes up to 10000 ---------- */
#define MAX_PRIME 10000
static uint32_t primes[2000];
static size_t prime_count = 0;
static unsigned char prime_ready = 0;

static void init_primes(void)
{
    if (prime_ready) return;
    unsigned char used[MAX_PRIME + 1];
    for (int i = 2; i * i <= MAX_PRIME; ++i) {
        if (!used[i]) {
            for (int j = i * i; j <= MAX_PRIME; j += i)
                used[j] = 1;
        }
    }
    for (int i = 2; i <= MAX_PRIME; ++i) {
        if (!used[i]) primes[prime_count++] = (uint32_t)i;
    }
    prime_ready = 1;
}

/* ---------- Sum of divisors sigma(m) via trial division ---------- */
static seq_t sigma_of(uint64_t m)
{
    if (m == 1) return (seq_t)1;
    seq_t sigma = (seq_t)1;
    uint64_t rem = m;
    init_primes();
    for (size_t i = 0; i < prime_count && (uint64_t)primes[i] * (uint64_t)primes[i] <= rem; ++i) {
        uint64_t p = primes[i];
        if (rem % p == 0) {
            int exp = 0;
            while (rem % p == 0) {
                rem /= p;
                ++exp;
            }
            seq_t term = (seq_t)1;
            seq_t powp = (seq_t)1;
            for (int j = 0; j < exp; ++j) {
                powp *= (seq_t)p;
                term += powp;
            }
            sigma *= term;
        }
    }
    if (rem > 1) { /* remainder is a prime factor > sqrt(original) */
        seq_t term = (seq_t)1 + (seq_t)rem;
        sigma *= term;
    }
    return sigma;
}

/* ---------- Main sequence function ---------- */
seq_t a(idx_t n)
{
    if (n < (idx_t)1) return (seq_t)0;   /* undefined for n < 1, return 0 */

    seq_t result = (seq_t)1;

    uint64_t N = (uint64_t)n;
    uint64_t sqrtN = int_sqrt_u64(N);

    for (uint64_t d = 1; d <= sqrtN; ++d) {
        if (N % d != 0) continue;

        if (d < N) {
            seq_t sig = sigma_of(d);
            seq_t def = (seq_t)2 * (seq_t)d - sig;
            result *= def;
        }

        uint64_t e = N / d;
        if (e != d && e < N) {
            seq_t sig = sigma_of(e);
            seq_t def = (seq_t)2 * (seq_t)e - sig;
            result *= def;
        }
    }

    return result;
}
