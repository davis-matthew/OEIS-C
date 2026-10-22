/*
 * A336736  Number of factorizations of n whose distinct factors have disjoint
 *          prime signatures.
 * https://oeis.org/A336736
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) = number of factorizations of n
   whose distinct factor values have disjoint prime signatures.
   The function is implemented by a depth‑first search over
   the divisor tree, checking signature conflicts as it goes. */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <math.h>

/* ---------- prime sieve (once per program) ---------- */

#define MAX_PRIME 10000   /* enough for all n <= 10000 */

static size_t _prime_cnt = 0;
static uint64_t _primes[2000];
static bool _primes_ready = false;

static void _init_primes(void)
{
    if (_primes_ready) return;
    bool mark[MAX_PRIME + 1] = { false };
    for (uint32_t i = 2; i <= MAX_PRIME; ++i) {
        if (!mark[i]) {
            _primes[_prime_cnt++] = i;
            if ((uint64_t)i * i <= MAX_PRIME) {
                for (uint32_t j = i * i; j <= MAX_PRIME; j += i) {
                    mark[j] = true;
                }
            }
        }
    }
    _primes_ready = true;
}

/* ---------- convert a value to a bit‑mask of exponent exponents ---------- */

static seq_t _mask_from_value(idx_t x)
{
    seq_t mask = 0;
    idx_t tmp = x;
    for (size_t i = 0; i < _prime_cnt && (idx_t)_primes[i] * (idx_t)_primes[i] <= tmp; ++i) {
        if (tmp % _primes[i] == 0) {
            unsigned exp = 0;
            while (tmp % _primes[i] == 0) {
                tmp /= _primes[i];
                ++exp;
            }
            /* set bit 'exp' */
            mask |= ((seq_t)1 << exp);
        }
    }
    if (tmp > 1) {          /* remaining prime factor with exponent 1 */
        mask |= ((seq_t)1 << 1);
    }
    return mask;
}

/* ---------- recursive DFS counting valid factorizations ---------- */

static seq_t _dfs(idx_t rem, idx_t min_f,
                  size_t distinct_cnt,
                  idx_t *val_arr, seq_t *mask_arr)
{
    seq_t total = 0;

    /* try all divisors d with min_f <= d <= sqrt(rem) */
    idx_t limit = (idx_t)(sqrt((double)rem) + 0.5);
    for (idx_t d = min_f; d <= limit; ++d) {
        if (rem % d != 0) continue;

        /* decide whether this d is a new distinct factor value */
        bool is_new = true;
        for (size_t i = 0; i < distinct_cnt; ++i) {
            if (val_arr[i] == d) { is_new = false; break; }
        }

        seq_t mask_d = 0;
        if (is_new) {
            /* check for signature conflict with existing distinct factors */
            mask_d = _mask_from_value(d);
            bool conflict = false;
            for (size_t i = 0; i < distinct_cnt; ++i) {
                if (mask_arr[i] & mask_d) { conflict = true; break; }
            }
            if (conflict) continue;            /* skip this divisor */

            /* record new distinct factor */
            val_arr[distinct_cnt] = d;
            mask_arr[distinct_cnt] = mask_d;
            ++distinct_cnt;
        }

        total += _dfs(rem / d, d, distinct_cnt, val_arr, mask_arr);

        if (is_new) --distinct_cnt;          /* pop the new factor */
    }

    /* consider using rem itself as the last factor */
    bool is_new = true;
    for (size_t i = 0; i < distinct_cnt; ++i) {
        if (val_arr[i] == rem) { is_new = false; break; }
    }
    if (is_new) {
        seq_t mask_rem = _mask_from_value(rem);
        bool conflict = false;
        for (size_t i = 0; i < distinct_cnt; ++i) {
            if (mask_arr[i] & mask_rem) { conflict = true; break; }
        }
        if (conflict) return total;
    }
    return total + 1;   /* a valid complete factorization has been found */
}

/* ---------- public function ---------- */

seq_t a(idx_t n)
{
    if (n <= 1) return 1;          /* a(1) = 1 by definition */

    _init_primes();

    /* arrays to hold distinct factor values and their masks; 20 is ample for n ≤ 10000 */
    idx_t val_arr[20];
    seq_t mask_arr[20];
    size_t distinct_cnt = 0;

    return _dfs(n, 2, distinct_cnt, val_arr, mask_arr);
}
