/*
 * A329029  a(n) = A069359(A276086(n)), where A276086 is the primorial base
 *          exp-function and A069359(n) = n * Sum_{p|n} 1/p.
 * https://oeis.org/A329029
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 58, u32 n <= 416, u64 n <= 2308,
 *            u128 n <= 2310, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = A069359( A276086(n) )
 * A276086(n) is the primorial‑base exponent function:
 *   Write n in primorial base:  n = Σ a_i · Π_{j<i} p_j,
 *   where a_i < p_i (p_i = i‑th prime).
 *   Then A276086(n) = Π p_i^{a_i}.
 * A069359(m) = m · Σ_{p|m} 1/p = Σ_{p|m} m/p.
 * The algorithm below converts n into its primorial–base digits,
 * constructs m, then sums m/p over all primes dividing m.
 */

#include <stdint.h>
#include <stddef.h>

/* Ensure <stdbool.h> only if needed for future helpers */
#include <stdbool.h>

static uint64_t _primes[128];
static size_t _prime_count = 0;

/* generate next prime and store in _primes */
static void _next_prime(void)
{
    static uint64_t candidate = 2;
    while (1) {
        bool prime = true;
        for (size_t i = 0; i < _prime_count; ++i) {
            uint64_t p = _primes[i];
            if (p * p > candidate)
                break;
            if (candidate % p == 0) {
                prime = false;
                break;
            }
        }
        if (prime) {
            _primes[_prime_count++] = candidate;
            ++candidate;
            return;
        }
        ++candidate;
    }
}

/* Compute a(n) */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* digits of n in primorial base */
    uint8_t digits[128] = {0};
    idx_t rem = n;
    idx_t weight = 1;
    size_t i = 0;

    while (rem > 0) {
        /* ensure we have prime i */
        if (i >= _prime_count)
            _next_prime();
        uint64_t p = _primes[i];

        /* get the i‑th digit */
        idx_t ai = rem / weight;
        ai %= p;                /* 0 <= ai < p */
        digits[i] = (uint8_t)ai;

        /* subtract contribution */
        rem -= ai * weight;

        /* update weight */
        if (weight > n / p) {
            weight = n + 1;     /* will make ai zero next loop */
        } else {
            weight *= p;
        }
        ++i;
    }

    /* construct A276086(n) = product p_i^{a_i} */
    seq_t m = 1;
    for (size_t j = 0; j < i; ++j) {
        uint8_t exp = digits[j];
        if (exp == 0)
            continue;
        seq_t pow = 1;
        for (uint8_t k = 0; k < exp; ++k)
            pow *= (seq_t)_primes[j];
        m *= pow;
    }

    /* compute A069359(m) = Σ m/p */
    seq_t sum = 0;
    for (size_t j = 0; j < i; ++j) {
        if (digits[j] == 0)
            continue;
        sum += m / (seq_t)_primes[j];
    }

    return sum;
}
