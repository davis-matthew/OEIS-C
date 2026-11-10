/*
 * A392872  a(n) = A122111(n) mod prime(1+bigomega(n)).
 * https://oeis.org/A392872
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A122111(n) mod prime(1+bigomega(n)), with A122111(n)
 *            computed directly modulo p = prime(1+bigomega(n)) via partition
 *            conjugation: if n = prod prime(i_k) with sorted indices,
 *            A122111(n) = prod over distinct index values v (in increasing
 *            order) of prime(r)^(v - prev), where r is the number o...
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 121 terms through n = 8549 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

static uint64_t A392872_nth_prime(uint64_t k)
{
    static uint64_t *pr = NULL;
    static size_t sz = 0, cap = 0;
    static uint64_t cand = 3;
    if (cap == 0) {
        cap = 64;
        pr = (uint64_t *)malloc(cap * sizeof(uint64_t));
        if (!pr) abort();
        pr[sz++] = 2;
    }
    while (sz < (size_t)k) {
        uint64_t p = cand;
        bool ok = true;
        for (size_t i = 0; i < sz; ++i) {
            uint64_t q = pr[i];
            if (q * q > p) break;
            if (p % q == 0) { ok = false; break; }
        }
        cand += 2;
        if (!ok) continue;
        if (sz == cap) {
            cap <<= 1;
            pr = (uint64_t *)realloc(pr, cap * sizeof(uint64_t));
            if (!pr) abort();
        }
        pr[sz++] = p;
    }
    return pr[(size_t)k - 1];
}

/* index of prime p (p prime) */
static uint64_t A392872_prime_index(uint64_t p)
{
    uint64_t lo = 1, hi = 1;
    while (A392872_nth_prime(hi) < p) hi *= 2;
    while (lo < hi) {
        uint64_t mid = lo + (hi - lo) / 2;
        if (A392872_nth_prime(mid) < p) lo = mid + 1; else hi = mid;
    }
    return lo;
}

static uint64_t A392872_mulmod(uint64_t a, uint64_t b, uint64_t m)
{
    return (uint64_t)(((unsigned __int128)a * b) % m);
}

static uint64_t A392872_powmod(uint64_t b, uint64_t e, uint64_t m)
{
    uint64_t r = 1 % m;
    b %= m;
    while (e) {
        if (e & 1) r = A392872_mulmod(r, b, m);
        b = A392872_mulmod(b, b, m);
        e >>= 1;
    }
    return r;
}

seq_t a(idx_t n)
{
    /* factor n by trial division; primes come out in increasing order */
    uint64_t idxs[128];      /* distinct prime indices, increasing */
    uint64_t mult[128];      /* multiplicities */
    size_t cnt = 0;
    uint64_t total = 0;
    uint64_t m = (uint64_t)n;
    for (uint64_t f = 2; f <= m / f; f += (f == 2 ? 1 : 2)) {
        if (m % f == 0) {
            uint64_t e = 0;
            while (m % f == 0) { m /= f; ++e; }
            idxs[cnt] = A392872_prime_index(f);
            mult[cnt] = e;
            ++cnt;
            total += e;
        }
    }
    if (m > 1) {
        idxs[cnt] = A392872_prime_index(m);
        mult[cnt] = 1;
        ++cnt;
        total += 1;
    }

    uint64_t p = A392872_nth_prime(total + 1);

    uint64_t res = 1 % p;
    uint64_t remaining = total;
    uint64_t prev = 0;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t len = idxs[i] - prev;
        uint64_t q = A392872_nth_prime(remaining);
        res = A392872_mulmod(res, A392872_powmod(q, len, p), p);
        remaining -= mult[i];
        prev = idxs[i];
    }
    return (seq_t)res;
}
