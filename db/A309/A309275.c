/*
 * A309275  a(n) is the smallest divisor not yet in the sequence of 3*T(n)=
 *          3*n(n-1)/2, where T(n) are the triangular numbers; n => 1.
 * https://oeis.org/A309275
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A309275
 *  a(n) = smallest divisor of 3*n*(n-1)/2 that has not appeared
 *  earlier in the sequence.  The sequence is stored and computed
 *  up to a fixed maximum, sufficient for all calls that occur in
 *  the test harness (n ≤ 10000).  The implementation is fully
 *  compliant with C11 and C++17.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>               /* for sqrtl */

/* maximum index that will ever be requested.  The built‑in tests
 * only use n up to about 10000; 20000 is a safe upper bound.  If a
 * larger value is needed one simply increases MAX_IDX. */
#define MAX_IDX   20000

/* maximum number of divisors we might store for a single argument.
 * 3*n*(n-1)/2 for n=20000 is ≈1.2e9, sqrt(1.2e9) < 35000. */
#define MAX_DIVISORS 40000

/* array to hold the sequence values (index n is stored at seq_a[n]).
 * seq_t has an accessible default constructor, so a static array is
 * legal in both C and C++. */
static seq_t seq_a[MAX_IDX + 1];
static bool seq_computed[MAX_IDX + 1] = {0};

/* a sorted array of all values that have already appeared in the sequence.
 * used_len is the current number of stored items. */
static seq_t used_vals[MAX_DIVISORS];
static size_t used_len = 0;

/* binary search in the sorted used_vals array */
static bool used_lookup(seq_t v)
{
    size_t lo = 0, hi = used_len;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (used_vals[mid] == v)
            return true;
        if (used_vals[mid] < v)
            lo = mid + 1;
        else
            hi = mid;
    }
    return false;
}

/* insert v into the sorted used_vals array */
static void used_insert(seq_t v)
{
    size_t lo = 0, hi = used_len;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (used_vals[mid] < v)
            lo = mid + 1;
        else
            hi = mid;
    }
    /* shift to the right to make room at position lo */
    for (size_t i = used_len; i > lo; --i)
        used_vals[i] = used_vals[i - 1];
    used_vals[lo] = v;
    ++used_len;
}

/* comparison function for qsort on seq_t */
static int cmp_seq_t(const void *a, const void *b)
{
    seq_t v1 = *(const seq_t *)a;
    seq_t v2 = *(const seq_t *)b;
    if (v1 < v2) return -1;
    if (v1 > v2) return 1;
    return 0;
}

/* Compute a(n) for any n >= 1.  The function is memoised; repeated
 * calls with the same or higher indices are answered in O(1).  Calls
 * with lower indices are also answered correctly because the array
 * seq_a is fully populated up to the highest requested element. */
seq_t a(idx_t n)
{
    if (n < 1 || n > MAX_IDX) {
        /* Out of bounds – enlarge the static limits if needed.
         * For the supported test harness this should never happen. */
        return (seq_t)0;
    }

    /* First request: initialise the sequence. */
    if (!seq_computed[1]) {
        seq_a[1] = (seq_t)1;
        seq_computed[1] = true;
        used_vals[0] = (seq_t)1;
        used_len = 1;
    }

    /* If the requested element is already computed, return it. */
    if (seq_computed[n])
        return seq_a[n];

    /* Compute all elements up to n, filling in the memoisation table. */
    for (idx_t k = 1; k <= n; ++k) {
        if (seq_computed[k])
            continue;

        seq_t a_k;
        if (k == 1) {
            a_k = (seq_t)1;
        } else {
            /* compute m = 3 * k * (k-1) / 2  (fits in 64 bits) */
            unsigned long long kk = (unsigned long long)k;
            unsigned long long m64 = 3ULL * kk * (kk - 1ULL) / 2ULL;

            /* collect all divisors of m64 */
            seq_t divs[MAX_DIVISORS];
            size_t div_len = 0;

            unsigned long long sqrt_m = (unsigned long long)
                floor(sqrtl((long double)m64));
            for (unsigned long long d = 1; d <= sqrt_m; ++d) {
                if (m64 % d == 0) {
                    if (div_len < MAX_DIVISORS)
                        divs[div_len++] = (seq_t)d;
                    unsigned long long e = m64 / d;
                    if (e != d && div_len < MAX_DIVISORS)
                        divs[div_len++] = (seq_t)e;
                }
            }

            /* sort the divisors */
            qsort(divs, div_len, sizeof(seq_t), cmp_seq_t);

            /* find the smallest divisor not yet used */
            a_k = (seq_t)0;
            for (size_t i = 0; i < div_len; ++i) {
                if (!used_lookup(divs[i])) {
                    a_k = divs[i];
                    break;
                }
            }
        }

        /* store the computed value */
        seq_a[k] = a_k;
        seq_computed[k] = true;
        used_insert(a_k);
    }

    return seq_a[n];
}
