/*
 * A296284  Solution of the complementary equation a(n) = a(n-1) + a(n-2) +
 *          n*b(n-2), where a(0) = 1, a(1) = 2, b(0) = 3, and (a(n)) and
 *          (b(n)) are increasing complementary sequences.
 * https://oeis.org/A296284
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=2, a(n)=a(n-1)+a(n-2)+n*b(n-2) (a(n)) and (b(n))
 *            are increasing complementary sequences, b(0)=3. This
 *            implementation memoises the sequences and builds them lazily
 * Range:     u8 n <= 6, u16 n <= 17, u32 n <= 40, u64 n <= 86, u128 n <= 178,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* external types supplied by the build */
extern seq_t a(idx_t n);

static seq_t *a_arr = NULL;
static idx_t a_len = 0, a_cap = 0;
static seq_t *b_arr = NULL;
static idx_t b_len = 0, b_cap = 0;

/* helpers to grow the dynamic arrays */
static void ensure_a_cap(idx_t needed) {
    if (a_cap < needed) {
        idx_t new_cap = a_cap ? a_cap << 1 : 4;
        while (new_cap < needed) new_cap <<= 1;
        a_arr = (seq_t *)realloc(a_arr, new_cap * sizeof(seq_t));
        a_cap = new_cap;
    }
}
static void ensure_b_cap(idx_t needed) {
    if (b_cap < needed) {
        idx_t new_cap = b_cap ? b_cap << 1 : 4;
        while (new_cap < needed) new_cap <<= 1;
        b_arr = (seq_t *)realloc(b_arr, new_cap * sizeof(seq_t));
        b_cap = new_cap;
    }
}

/* state for generating b: next candidate for complement and
   index of next a that might equal the candidate */
static seq_t cur_candidate = 1;
static idx_t next_a_idx = 0;

/* generates the next complement number and stores it in b_arr */
static void generate_next_b(void) {
    seq_t cand = cur_candidate;
    while (next_a_idx < a_len && a_arr[next_a_idx] <= cand) {
        if (a_arr[next_a_idx] == cand) {
            cand++;
            next_a_idx++;
        } else {
            next_a_idx++;
        }
    }
    ensure_b_cap(b_len + 1);
    b_arr[b_len++] = cand;
    cur_candidate = cand + 1;
}

/* Build a and b up to at least index n */
static void build_up_to(idx_t n) {
    if (a_len > 0 && a_len > n) return;  /* already built */

    /* initialise at first call */
    if (a_len == 0) {
        ensure_a_cap(2);
        ensure_b_cap(1);
        a_arr[0] = 1;
        a_arr[1] = 2;
        a_len = 2;

        cur_candidate = 1;
        next_a_idx = 0;
        generate_next_b();          /* b(0) = 3 */
    }

    for (idx_t i = a_len; i <= n; ++i) {
        /* ensure b(i-2) exists */
        while (b_len <= i - 2) generate_next_b();

        seq_t val = a_arr[i - 1] + a_arr[i - 2] + (seq_t)i * b_arr[i - 2];
        ensure_a_cap(i + 1);
        a_arr[i] = val;
        ++a_len;
    }
}

seq_t a(idx_t n) {
    build_up_to(n);
    return a_arr[n];
}
