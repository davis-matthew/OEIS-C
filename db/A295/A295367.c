/*
 * A295367  Solution of the complementary equation a(n) = a(n-1) + a(n-2) +
 *          b(n-1)*b(n-2), where a(0) = 1, a(1) = 2, b(0) = 3, b(1) = 4, and
 *          (a(n)) and (b(n)) are increasing complementary sequences.
 * https://oeis.org/A295367
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 16, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS A295367 – complementary sequences a(n) & b(n)

   a(n) = a(n-1) + a(n-2) + b(n-1)*b(n-2)
   a(0)=1, a(1)=2
   b(0)=3, b(1)=4
   a() and b() are increasing and complementary (cover all positives).
   Each b(n) is the least positive integer not already appearing in either sequence.
*/

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* binary search: return true if val is present in sorted array arr[0..len-1] */
static inline bool binary_search(const seq_t *arr, size_t len, seq_t val) {
    size_t lo = 0, hi = len;
    while (lo < hi) {
        size_t mid = lo + (hi - lo) / 2;
        if (arr[mid] < val) {
            lo = mid + 1;
        } else {
            hi = mid;
        }
    }
    return (lo < len && arr[lo] == val);
}

/* tests whether val occurs in sequences a or b */
static inline bool is_used(const seq_t *a, size_t a_len,
                           const seq_t *b, size_t b_len,
                           seq_t val) {
    return binary_search(a, a_len, val) || binary_search(b, b_len, val);
}

/* the public function */
seq_t a(idx_t n) {
    /* static storage for dynamic vectors */
    static seq_t *a_seq = NULL;
    static size_t a_len = 0, a_cap = 0;
    static seq_t *b_seq = NULL;
    static size_t b_len = 0, b_cap = 0;
    static seq_t next_candidate = 1;
    static bool initialized = false;

    /* initialise with first four terms */
    if (!initialized) {
        a_cap = 4;
        b_cap = 4;
        a_seq = (seq_t *)malloc(a_cap * sizeof(seq_t));
        b_seq = (seq_t *)malloc(b_cap * sizeof(seq_t));
        if (!a_seq || !b_seq) abort();  /* allocation failure */

        a_seq[0] = (seq_t)1;   /* a(0) */
        a_seq[1] = (seq_t)2;   /* a(1) */
        b_seq[0] = (seq_t)3;   /* b(0) */
        b_seq[1] = (seq_t)4;   /* b(1) */
        a_len = 2;
        b_len = 2;

        /* initialise next_candidate to first unused number after init */
        next_candidate = (seq_t)1;
        while (is_used(a_seq, a_len, b_seq, b_len, next_candidate))
            next_candidate++;

        initialized = true;
    }

    /* extend sequences up to requested n */
    for (idx_t i = a_len; i <= n; ++i) {
        /* compute a(i) */
        seq_t an = a_seq[i - 1] + a_seq[i - 2] + b_seq[i - 1] * b_seq[i - 2];

        /* ensure capacity for a_seq */
        if (a_len == a_cap) {
            a_cap = a_cap ? a_cap << 1 : 4;
            seq_t *tmp = (seq_t *)realloc(a_seq, a_cap * sizeof(seq_t));
            if (!tmp) abort();
            a_seq = tmp;
        }
        a_seq[a_len++] = an;

        /* compute b(i) as the least unused natural number */
        /* next_candidate is already the smallest unused integer */
        seq_t bn = next_candidate;

        /* ensure capacity for b_seq */
        if (b_len == b_cap) {
            b_cap = b_cap ? b_cap << 1 : 4;
            seq_t *tmp = (seq_t *)realloc(b_seq, b_cap * sizeof(seq_t));
            if (!tmp) abort();
            b_seq = tmp;
        }
        b_seq[b_len++] = bn;

        /* advance next_candidate to the next unused value */
        do {
            next_candidate++;
        } while (is_used(a_seq, a_len, b_seq, b_len, next_candidate));
    }

    return a_seq[n];
}
