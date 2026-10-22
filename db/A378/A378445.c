/*
 * A378445  a(n) is the number of divisors d of n such that A083345(d) is odd,
 *          where A083345(n) is the numerator of Sum(e/p: n=Product(p^e)).
 * https://oeis.org/A378445
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of divisors d of n such that A083345(d) is odd.
 *            A083345(d) is the numerator of sum(e/p: d = ∏ p^e). This
 *            implementation factorises n, enumerates all divisors, computes
 *            the numerator modulo 4 and counts the divisors for which that
 *            numerator is odd
 * Range:     u8 n <= 9865, u16 n <= 9865, u32 n <= 9865, u64 n <= 9865,
 *            u128 n <= 9865, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Forward declaration of the helper recursion */
static void rec_divs(size_t idx, size_t prime_cnt,
                     const uint64_t primes[], const uint64_t exps[],
                     uint64_t cur_exps[], size_t idx2, seq_t *cnt);

/* Main sequence function */
seq_t a(idx_t n)
{
    /* factorise n (n <= 10000 for the performance requirement) */
    uint64_t facs[16];
    uint64_t exps[16];
    size_t prime_cnt = 0;
    uint64_t tmp = (uint64_t)n;
    if (tmp % 2 == 0) {
        facs[prime_cnt] = 2;
        uint64_t c = 0;
        while (tmp % 2 == 0) {
            tmp /= 2;
            ++c;
        }
        exps[prime_cnt++] = c;
    }
    for (uint64_t p = 3; p * p <= tmp; p += 2) {
        if (tmp % p == 0) {
            facs[prime_cnt] = p;
            uint64_t c = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++c;
            }
            exps[prime_cnt++] = c;
        }
    }
    if (tmp > 1) {
        facs[prime_cnt] = tmp;
        exps[prime_cnt] = 1;
        ++prime_cnt;
    }

    /* index of prime 2 if present, otherwise SIZE_MAX */
    size_t idx2 = (size_t)-1;
    for (size_t i = 0; i < prime_cnt; ++i) {
        if (facs[i] == 2) {
            idx2 = i;
            break;
        }
    }

    /* current exponents for recursion */
    uint64_t cur_exps[16] = {0};

    seq_t cnt = 0;
    rec_divs(0, prime_cnt, facs, exps, cur_exps, idx2, &cnt);
    return cnt;
}

/* Recursively enumerate all divisors of n and count the desired ones */
static void rec_divs(size_t idx, size_t prime_cnt,
                     const uint64_t primes[], const uint64_t exps[],
                     uint64_t cur_exps[], size_t idx2, seq_t *cnt)
{
    if (idx == prime_cnt) {
        /* compute m_d = product of distinct primes with exponent > 0 */
        uint64_t m = 1;
        for (size_t i = 0; i < prime_cnt; ++i) {
            if (cur_exps[i] > 0)
                m *= primes[i];
        }

        /* compute numerator modulo 4: N = Σ e_i * (m / p_i) */
        uint64_t N = 0;
        for (size_t i = 0; i < prime_cnt; ++i) {
            if (cur_exps[i] > 0) {
                uint64_t term = cur_exps[i] * (m / primes[i]);
                N += term;
            }
        }
        uint64_t N_mod4 = N & 3U;      /* N % 4 */

        bool has2 = (idx2 != (size_t)-1) && (cur_exps[idx2] > 0);
        if ((!has2 && (N_mod4 & 1U)) || (has2 && N_mod4 != 0U))
            (*cnt)++;
        return;
    }

    /* iterate over all exponent options for this prime */
    for (uint64_t e = 0; e <= exps[idx]; ++e) {
        cur_exps[idx] = e;
        rec_divs(idx + 1, prime_cnt, primes, exps, cur_exps, idx2, cnt);
    }
}
