/*
 * A368904  Numbers k for which there is no prime p such that p^p divides
 *          A342001(k).
 * https://oeis.org/A368904
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k >= 1 such that A342001(k) is not divisible by p^p
 *            for any prime p (A342001(k) in A048103), where A342001(k) =
 *            rad(k)*Sum_{p^e||k} e/p; found by search
 * Range:     u8 n <= 81, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Validated: partial, pass, 81 terms through n = 81 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

static seq_t A342001_val(idx_t n)
{
    if (n <= 1)
        return (seq_t)0;
    idx_t primes[64];
    idx_t exps[64];
    size_t fcnt = 0;
    idx_t t = n;
    for (idx_t p = 2; p <= t / p; ++p) {
        if (t % p == 0) {
            idx_t e = 0;
            while (t % p == 0) {
                t /= p;
                ++e;
            }
            primes[fcnt] = p;
            exps[fcnt] = e;
            ++fcnt;
        }
    }
    if (t > 1) {
        primes[fcnt] = t;
        exps[fcnt] = 1;
        ++fcnt;
    }
    seq_t rad = (seq_t)1;
    for (size_t i = 0; i < fcnt; ++i)
        rad *= (seq_t)primes[i];
    seq_t result = (seq_t)0;
    for (size_t i = 0; i < fcnt; ++i) {
        seq_t term = rad / (seq_t)primes[i];
        result += (seq_t)exps[i] * term;
    }
    return result;
}

static bool A368904_good(idx_t k)
{
    seq_t v = A342001_val(k);
    if (v == 0)
        return false;
    for (unsigned p = 2;; ++p) {
        seq_t pw = 1;
        bool fits = true;
        for (unsigned i = 0; i < p; ++i) {
            if (pw > v / (seq_t)p) {
                fits = false;
                break;
            }
            pw *= (seq_t)p;
        }
        if (!fits)
            break;
        if (v % pw == 0)
            return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t k = 1;
    while (count < n) {
        ++k;
        if (A368904_good(k))
            ++count;
    }
    return (seq_t)k;
}
