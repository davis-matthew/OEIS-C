/*
 * A370122  a(n) = 1 if the second arithmetic derivative of n is a multiple of
 *          3, otherwise 0.
 * https://oeis.org/A370122
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if the second arithmetic derivative of n is a multiple
 *            of 3, otherwise 0. Edge case: a(0) is defined as 1
 * Range:     u8 n <= 107, u16 n <= 9260, u32 n <= 9260, u64 n <= 9260,
 *            u128 n <= 9260, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* -------------- small prime table, built once ----------------- */
static uint32_t prime_table[20000];
static size_t prime_count = 0;
static int primes_built = 0;
static char sieve_flag[200001];

static void build_small_primes(void)
{
    if (primes_built) return;
    primes_built = 1;
    size_t idx = 0;
    for (uint32_t i = 2; i <= 200000; ++i) {
        if (!sieve_flag[i]) {
            prime_table[idx++] = i;
            if ((uint64_t)i * i <= 200000) {
                for (uint32_t j = i * i; j <= 200000; j += i) sieve_flag[j] = 1;
            }
        }
    }
    prime_count = idx;
}

/* -------------- arithmetic derivative -------------------------- */
static seq_t arithmetic_derivative(seq_t x)
{
    if (x <= (seq_t)1) return (seq_t)0; /* 0' = 0, 1' = 0 */

    seq_t result = (seq_t)0;
    idx_t rem = (idx_t)x;            /* we factor small numbers; cast is safe for n <= 10000 */

    build_small_primes();

    for (size_t i = 0; i < prime_count && (uint64_t)prime_table[i] * prime_table[i] <= rem; ++i) {
        uint32_t p = prime_table[i];
        if (rem % p == 0) {
            size_t exp = 0;
            while (rem % p == 0) {
                ++exp;
                rem /= p;
            }
            result += (seq_t)exp * (x / (seq_t)p);
        }
    }
    if (rem > 1) {                /* remaining prime factor */
        uint32_t p = (uint32_t)rem;
        result += (seq_t)1 * (x / (seq_t)p);
    }
    return result;
}

/* -------------- sequence a(n) --------------------------------- */
seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)1;   /* definition for n = 0 */

    seq_t d1 = arithmetic_derivative((seq_t)n);
    seq_t d2 = arithmetic_derivative(d1);

    return (d2 % (seq_t)3 == (seq_t)0) ? (seq_t)1 : (seq_t)0;
}
