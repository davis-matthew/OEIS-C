/*
 * A321657  Base-4 deletable primes (written in base 10).
 * https://oeis.org/A321657
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is the nth base‑4 deletable prime (OEIS A321657)
 * Range:     u8 n <= 40, u16 n <= 2796, u32 n <= 2796, u64 n <= 2796,
 *            u128 n <= 2796, big any n
 * Validated: standard, pass, 72 terms through n = 2494 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/mem.h"

/* Helper: binary search on a sorted array of seq_t */
static bool deletable_contains(const seq_t *arr, size_t cnt, seq_t key)
{
    size_t lo = 0, hi = cnt;
    while (lo < hi)
    {
        size_t mid = (lo + hi) >> 1;
        if (arr[mid] == key) return true;
        if (arr[mid] < key) lo = mid + 1; else hi = mid;
    }
    return false;
}

/* Helper: test if a prime p is base‑4 deletable using current deletable list */
static bool is_deletable(seq_t p, const seq_t *deletable, size_t deletableCnt)
{
    seq_t tmp = p;
    uint8_t digits[128];          /* 128 base‑4 digits are enough for all tested n */
    size_t len = 0;

    if (tmp == 0) return false;   /* not a prime */
    while (tmp > 0)
    {
        digits[len++] = (uint8_t)(tmp % 4);
        tmp /= 4;
    }

    if (len == 1) return true;    /* 2 and 3: single digit, deletion gives empty string */

    for (size_t pos = 0; pos < len; ++pos)
    {
        /* removing the most significant digit would leave a leading zero? */
        if (pos == len - 1 && len > 1 && digits[len - 2] == 0)
            continue;

        seq_t q = 0;
        for (size_t idx = len; idx-->0; )
        {
            if (idx == pos) continue;
            q = q * 4 + digits[idx];
        }
        if (deletable_contains(deletable, deletableCnt, q))
            return true;
    }
    return false;
}

/* Returns the n-th base‑4 deletable prime, n >= 1 */
seq_t a(idx_t n)
{
    /* static state that is preserved between calls */
    static seq_t *primes = NULL;
    static size_t primesCnt = 0, primesCap = 0;
    static seq_t *deletable = NULL;
    static size_t deletableCnt = 0, deletableCap = 0;
    static seq_t nextCandidate = 2;

    /* ---------- ensure storage capacities ---------- */
    if (primesCap == 0) { primesCap = 16; primes = mem_new(seq_t, primesCap); }
    if (deletableCap == 0) { deletableCap = 16; deletable = mem_new(seq_t, deletableCap); }

    /* ---------- generate until we have n deletable primes ---------- */
    for (;;)
    {
        if (deletableCnt >= (size_t)n)
            break;   /* already enough computed */

        /* skip even numbers > 2 to reduce work */
        if (nextCandidate > 2 && (nextCandidate & 1) == 0)
        {
            ++nextCandidate;
            continue;
        }

        /* primality test: trial division by known primes */
        bool prime = true;
        if (nextCandidate < 2) prime = false;
        else
        {
            for (size_t i = 0; i < primesCnt; ++i)
            {
                seq_t p = primes[i];
                if (p * p > nextCandidate) break;
                if (nextCandidate % p == 0) { prime = false; break; }
            }
        }

        if (prime)
        {
            /* add to primes list */
            if (primesCnt == primesCap)
            {
                primesCap <<= 1;
                seq_t *tmp = mem_resize(seq_t, primes, primesCap);
                if (!tmp) abort();
                primes = tmp;
            }
            primes[primesCnt++] = nextCandidate;

            /* check deletability */
            if (is_deletable(nextCandidate, deletable, deletableCnt))
            {
                if (deletableCnt == deletableCap)
                {
                    deletableCap <<= 1;
                    seq_t *tmp = mem_resize(seq_t, deletable, deletableCap);
                    if (!tmp) abort();
                    deletable = tmp;
                }
                deletable[deletableCnt++] = nextCandidate;
            }
        }

        ++nextCandidate;
    }

    return deletable[(size_t)n - 1];
}
