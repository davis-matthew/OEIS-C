/*
 * A297826  Solution (a(n)) of the near-complementary equation a(n) =
 *          a(1)*b(n-1) - a(0)*b(n-2) + n, where a(0) = 1, a(1) = 2, b(0) = 3,
 *          b(1) = 4, and (b(n)) is the increasing sequence of positive
 *          integers not in (a(n)). See Comments.
 * https://oeis.org/A297826
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 97, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A297826
    Recurrence: a(0)=1, a(1)=2;
    a(n)=a(1)*b(n-1)-a(0)*b(n-2)+n  (n≥2)
    b(n) = increasing positive integers not in the sequence a.
    The implementation uses memoisation so that calls to a(idx_t n)
    in any order are inexpensive.
*/
#if defined(__cplusplus)
#   include <vector>
#   include <cstddef>
#   include <cstdint>
#else
#   include <stdlib.h>
#   include <stdbool.h>
#endif

static seq_t next_candidate = 1;   /* next integer to test for b(n) */

#ifdef __cplusplus
/* C++ implementation with std::vector */
static std::vector<seq_t> a_cache;
static std::vector<seq_t> b_cache;

/* binary search – return true if val is present in a_cache */
static inline bool a_contains(seq_t val)
{
    size_t lo = 0, hi = a_cache.size();
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (a_cache[mid] < val) lo = mid + 1;
        else if (a_cache[mid] > val) hi = mid;
        else return true;
    }
    return false;
}

/* generate the next b(n) value */
static seq_t next_b_value(void)
{
    seq_t val = next_candidate;
    while (true) {
        if (a_contains(val)) {
            ++val;          /* skip if a already contains it */
            continue;
        }
        /* val is not in a; it becomes the next b value */
        next_candidate = val + 1;
        return val;
    }
}

/* ensure sequences are computed up to index n (inclusive) */
static void ensure_sequences(idx_t n)
{
    if (a_cache.empty()) {          /* first time initialisation */
        a_cache.reserve(2);
        a_cache.push_back((seq_t)1);
        a_cache.push_back((seq_t)2);

        b_cache.reserve(2);
        b_cache.push_back(next_b_value());   /* 3 */
        b_cache.push_back(next_b_value());   /* 4 */
    }

    while (a_cache.size() <= (size_t)n) {
        size_t cur = a_cache.size();          /* index of new element */

        /* ensure b_cache has entries up to cur-1 */
        while (b_cache.size() <= cur - 1)
            b_cache.push_back(next_b_value());

        seq_t new_val = a_cache[1] * b_cache[cur - 1] -
                        a_cache[0] * b_cache[cur - 2] +
                        (seq_t)cur;
        a_cache.push_back(new_val);
    }
}

seq_t a(idx_t n)
{
    ensure_sequences(n);
    return a_cache[(size_t)n];
}

#else  /* C implementation with malloc/realloc */

/* C implementation using malloc/realloc. All indices are size_t. */
static seq_t *a_cache = NULL;
static seq_t *b_cache = NULL;
static size_t a_count = 0, a_cap = 0;
static size_t b_count = 0, b_cap = 0;

/* grow an array if needed (realloc is safe for plain integer seq_t) */
static void ensure_capacity_seq(seq_t **arr, size_t *cap, size_t need)
{
    if (need > *cap) {
        size_t ncap = (*cap > 0) ? (*cap << 1) : 8;
        if (ncap < need) ncap = need;
        *arr = realloc(*arr, ncap * sizeof(seq_t));
        *cap = ncap;
    }
}

/* binary search – return true if val is present in a_cache */
static inline bool a_contains(seq_t val)
{
    size_t lo = 0, hi = a_count;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (a_cache[mid] < val) lo = mid + 1;
        else if (a_cache[mid] > val) hi = mid;
        else return true;
    }
    return false;
}

/* generate the next b(n) value */
static seq_t next_b_value(void)
{
    seq_t val = next_candidate;
    while (true) {
        if (a_contains(val)) {
            ++val;          /* skip if a already contains it */
            continue;
        }
        next_candidate = val + 1;
        return val;
    }
}

/* ensure sequences are computed up to index n (inclusive) */
static void ensure_sequences(idx_t n)
{
    if (a_count == 0) {          /* first time initialisation */
        ensure_capacity_seq(&a_cache, &a_cap, 2);
        a_cache[0] = (seq_t)1;
        a_cache[1] = (seq_t)2;
        a_count = 2;

        ensure_capacity_seq(&b_cache, &b_cap, 2);
        b_cache[0] = next_b_value();   /* 3 */
        b_cache[1] = next_b_value();   /* 4 */
        b_count = 2;
    }

    while (a_count <= (size_t)n) {
        size_t cur = a_count;            /* index of new element */

        /* ensure b_cache has entries up to cur-1 */
        while (b_count <= cur - 1) {
            ensure_capacity_seq(&b_cache, &b_cap, b_count + 1);
            b_cache[b_count++] = next_b_value();
        }

        seq_t new_val = a_cache[1] * b_cache[cur - 1] -
                        a_cache[0] * b_cache[cur - 2] +
                        (seq_t)cur;
        ensure_capacity_seq(&a_cache, &a_cap, a_count + 1);
        a_cache[a_count++] = new_val;
    }
}

seq_t a(idx_t n)
{
    ensure_sequences(n);
    return a_cache[(size_t)n];
}

#endif   /* end of C / C++ conditional */
