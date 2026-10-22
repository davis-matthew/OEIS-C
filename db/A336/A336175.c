/*
 * A336175  Numbers k such that there are no powerful numbers between k^2 and
 *          (k+1)^2.
 * https://oeis.org/A336175
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 616, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   seq_alloc, seq_cmp
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the sequence A336175:
   a(n) = the n-th integer k such that there is no powerful number
   strictly between k² and (k+1)².
   The implementation pre‑computes all powerful numbers up to a
   moderate limit and then builds the sequence by scanning k
   in ascending order.  It caches the sequence in static arrays so
   that calls are fast even if they are not monotone.  */

#include <stdlib.h>
#include <stdbool.h>

/* Use the utilities provided by the framework.  */
#include "../utils/seq_alloc.h"       /* seq_alloc(size_t) -> seq_t*   */
#include "../utils/seq_cmp.h"         /* int seq_cmp(const void*,const void*) */

/* -------------------------------------------------------------------------- */
/* Types supplied by the build system:  seq_t, idx_t.  We rely on them only
 * via the allowed operations (+ - * / % and the usual comparisons).  */

static bool _initialised = false;

/* List of powerful numbers, sorted and deduplicated.  */
static seq_t *_pow_nums = NULL;
static size_t _pow_len  = 0;

/* Array of the sequence values a(1), a(2), …  */
static seq_t *_seq_vals = NULL;
static size_t _seq_len  = 0;

/* -------------------------------------------------------------------------- */
/* Constants -------------------------------------------------------------- */
/* The maximum k that we consider when pre‑computing.  This is more than
 * enough for any reasonably small n (< ~120 000).  */
#define MAX_K  50000ULL
/* Upper bound for the powerful numbers we generate.  */
static const seq_t LIMIT = ((seq_t)(MAX_K + 1U)) * ((seq_t)(MAX_K + 1U));
/* Size of the pre‑computed sequence array.  Should be large enough to
 * contain all terms that fit in the chosen seq_t type.  */
#define SEQ_CAP 120000

/* -------------------------------------------------------------------------- */
/* Helper: binary search that returns the first index i such that
 * _pow_nums[i] > key (or _pow_len if none).  */
static size_t _upper_bound_powerful (seq_t key)
{
    size_t lo = 0, hi = _pow_len;
    while (lo < hi) {
        size_t mid = lo + (hi - lo) / 2;
        if (_pow_nums[mid] <= key)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo;  /* may equal _pow_len */
}

/* -------------------------------------------------------------------------- */
/* Generate all numbers of the form a²·b³ that do not exceed LIMIT.
 * The array is sorted and duplicates are removed.  */
static void _generate_powerful (void)
{
    /* Initial allocation; grow as needed.  */
    size_t cap = 200000;                       /* rough upper bound */
    _pow_nums = seq_alloc(cap);
    _pow_len  = 0;

    for (uint64_t a = 1; a * a <= (uint64_t)LIMIT; ++a) {
        uint64_t a2 = a * a;
        for (uint64_t b = 1;; ++b) {
            uint64_t b3 = b * b * b;
            /* Check for overflow against LIMIT.  */
            if (a2 > (uint64_t)LIMIT / b3)
                break;
            seq_t val = (seq_t)(a2 * b3);
            if (_pow_len == cap) {            /* grow the array */
                cap <<= 1;
                _pow_nums = (seq_t*)realloc(_pow_nums, cap * sizeof(seq_t));
            }
            _pow_nums[_pow_len++] = val;
        }
    }

    /* Sort the array.  */
    qsort(_pow_nums, _pow_len, sizeof(seq_t), seq_cmp);

    /* Remove duplicates in place.  */
    size_t uniq = 0;
    for (size_t i = 0; i < _pow_len; ++i)
        if (uniq == 0 || _pow_nums[i] != _pow_nums[uniq - 1])
            _pow_nums[uniq++] = _pow_nums[i];
    _pow_len = uniq;
}

/* -------------------------------------------------------------------------- */
/* Pre‑compute the sequence up to SEQ_CAP terms.  */
static void _precompute_sequence (void)
{
    _seq_vals = seq_alloc(SEQ_CAP);
    _seq_len  = 0;

    for (uint64_t k = 1; _seq_len < SEQ_CAP; ++k) {
        seq_t k2 = (seq_t)k * (seq_t)k;
        size_t idx = _upper_bound_powerful(k2);

        bool has_powerful_between = false;
        if (idx < _pow_len) {
            seq_t next = _pow_nums[idx];
            seq_t next_sq = (seq_t)(k + 1U) * (seq_t)(k + 1U);
            if (next < next_sq)
                has_powerful_between = true;
        }

        if (!has_powerful_between)
            _seq_vals[_seq_len++] = (seq_t)k;
    }
}

/* -------------------------------------------------------------------------- */
/* Main entry point.  */
seq_t a (idx_t n)
{
    if (!_initialised) {
        _initialised = true;
        _generate_powerful();
        _precompute_sequence();
    }

    if (n >= 1 && (size_t)n <= _seq_len)
        return _seq_vals[(size_t)n - 1];

    /* If n exceeds the pre‑computed range, continue scanning k
     * until the nth term is found.  */
    size_t count = 0;
    for (uint64_t k = 1;; ++k) {
        seq_t k2 = (seq_t)k * (seq_t)k;
        size_t idx = _upper_bound_powerful(k2);
        bool has_powerful_between = false;
        if (idx < _pow_len) {
            seq_t next = _pow_nums[idx];
            seq_t next_sq = (seq_t)(k + 1U) * (seq_t)(k + 1U);
            if (next < next_sq)
                has_powerful_between = true;
        }
        if (!has_powerful_between) {
            ++count;
            if (count == (size_t)n)
                return (seq_t)k;
        }
    }
}
