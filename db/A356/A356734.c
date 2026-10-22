/*
 * A356734  Heinz numbers of integer partitions with at least one neighborless
 *          part.
 * https://oeis.org/A356734
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth smallest integer m >= 2 whose prime‑index
 *            multiset (obtained from the factorization of m) contains at
 *            least one isolated index, i.e., an index x for which neither x-1
 *            nor x+1 occurs in the multiset
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* static tables and initialization flag ----------------------------------- */
static bool _a_initialized = false;
static uint32_t * _a_spf   = NULL;      /* smallest prime factor for each int */
static uint32_t * _a_pri   = NULL;      /* prime index for each prime          */
static uint32_t * _a_sp   = NULL;      /* list of primes                      */
static size_t    _a_sp_cnt = 0;       /* number of primes stored             */

/* initialize spf, prime indices and prime list up to LIMIT */
static void _a_init(void)
{
    const uint32_t LIMIT = 5000000; /* safe for n up to ~10⁴, can be raised if needed */
    _a_spf = (uint32_t*)malloc((LIMIT + 1) * sizeof(uint32_t));
    _a_pri = (uint32_t*)malloc((LIMIT + 1) * sizeof(uint32_t));
    _a_sp  = (uint32_t*)malloc((LIMIT / 10 + 100) * sizeof(uint32_t)); /* rough size */

    for (uint32_t i = 0; i <= LIMIT; ++i) _a_spf[i] = i;
    _a_pri[0] = _a_pri[1] = 0;

    size_t pctr = 0;
    for (uint32_t i = 2; i <= LIMIT; ++i) {
        if (_a_spf[i] == i) {                  /* i is prime */
            _a_sp[pctr++] = i;
            _a_pri[i] = (uint32_t)(pctr);      /* 1‑based index */
            if ((uint64_t)i * i <= LIMIT) {
                for (uint32_t j = i * i; j <= LIMIT; j += i)
                    if (_a_spf[j] == j) _a_spf[j] = i;
            }
        }
    }
    _a_sp_cnt = pctr;
    _a_initialized = true;
}

/* main sequence function --------------------------------------------------- */
seq_t a(idx_t n)
{
    if (!_a_initialized) _a_init();

    idx_t found = 0;        /* how many qualifying integers we have seen */
    uint64_t num = 2;       /* start iterating from 2 */

    while (true) {
        /* factorization: gather unique prime indices in ascending order   */
        uint32_t indices[64];
        int len = 0;
        uint64_t t = num;

        while (t > 1) {
            uint32_t p = _a_spf[(uint32_t)t];
            uint32_t idx = _a_pri[p];
            /* store only the first occurrence of each index */
            if (len == 0 || indices[len - 1] != idx)
                indices[len++] = idx;
            while (t % p == 0) t /= p;
        }

        /* check for at least one isolated index */
        bool isolated = false;
        for (int i = 0; i < len; ++i) {
            bool left  = (i > 0)   && (indices[i] == indices[i - 1] + 1);
            bool right = (i + 1 < len) && (indices[i] == indices[i + 1] - 1);
            if (!left && !right) { isolated = true; break; }
        }

        if (isolated) {
            ++found;
            if (found == n) return (seq_t)num;
        }
        ++num;
    }
}
