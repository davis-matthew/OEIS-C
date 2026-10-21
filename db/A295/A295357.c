/*
 * A295357  Solution of the complementary equation a(n) = a(n-1) + a(n-2) +
 *          b(n-1) + b(n-2) + b(n-3), where a(0) = 1, a(1) = 3, a(2) = 5, b(0)
 *          = 2, b(1) = 4, b(2) = 6, and (a(n)) and (b(n)) are increasing
 *          complementary sequences.
 * https://oeis.org/A295357
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 18, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS A295357: Complementary sequences a(n) and b(n)
 *
 * Recurrence:
 *   a(0)=1, a(1)=3, a(2)=5
 *   b(0)=2, b(1)=4, b(2)=6
 *   for n ≥ 3:
 *     a(n) = a(n-1)+a(n-2)+b(n-1)+b(n-2)+b(n-3)
 *
 * The sequence a(n) is computed together with its complementary
 * sequence b(n), which is assembled by scanning natural numbers
 * and skipping those that already appear in a.
 *
 * The function a(idx_t n) is thread‑local only in the sense that the
 * static buffers are not protected; for single‑threaded use it satisfies
 * all specified requirements.
 */

#include <stdlib.h>
#include <stdbool.h>

/* static storage for the running sequences */
static seq_t *a_seq   = NULL;
static seq_t *b_seq   = NULL;
static size_t a_len   = 0;   /* number of a values computed          */
static size_t b_len   = 0;   /* number of b values computed          */
static size_t cap     = 0;   /* current capacity of the arrays       */
static idx_t  next_candidate = 2;   /* next integer to try for b          */
static size_t a_pos   = 0;           /* index into a_seq used for generating b */

/* ensure that both arrays can hold at least 'need' elements */
static void ensure_cap(size_t need)
{
    if (need <= cap)
        return;
    size_t new_cap = cap ? cap << 1 : 16;
    while (new_cap < need)
        new_cap <<= 1;
    a_seq = (seq_t*)realloc(a_seq, new_cap * sizeof(seq_t));
    b_seq = (seq_t*)realloc(b_seq, new_cap * sizeof(seq_t));
    cap = new_cap;
}

/* generate the next element of the complementary sequence b */
static void generate_next_b(void)
{
    /* advance a_pos until a_seq[a_pos] >= next_candidate */
    while (a_pos < a_len && a_seq[a_pos] < next_candidate)
        ++a_pos;

    /* skip any a value that equals the current candidate */
    while (a_pos < a_len && a_seq[a_pos] == next_candidate) {
        ++next_candidate;
        ++a_pos;
    }

    /* store the candidate as the next b element */
    ensure_cap(b_len + 1);
    b_seq[b_len] = (seq_t)next_candidate;
    ++next_candidate;
    ++b_len;
}

/* public function: computes a(n) for any n ≥ 0 */
seq_t a(idx_t n)
{
    static bool init = false;
    if (!init) {
        cap = 16;
        a_seq = (seq_t*)calloc(cap, sizeof(seq_t));
        b_seq = (seq_t*)calloc(cap, sizeof(seq_t));

        /* initial values of a */
        a_seq[0] = (seq_t)1;
        a_seq[1] = (seq_t)3;
        a_seq[2] = (seq_t)5;
        a_len = 3;

        /* build initial b values (b0 … b2) */
        while (b_len < 3)
            generate_next_b();

        init = true;
    }

    /* base cases handled directly */
    if (n == 0u) return (seq_t)1;
    if (n == 1u) return (seq_t)3;
    if (n == 2u) return (seq_t)5;

    /* extend sequences up to n if required */
    if ((size_t)n >= a_len) {
        for (size_t i = a_len; i <= (size_t)n; ++i) {
            /* ensure required b indices are available */
            while (b_len <= i - 1)
                generate_next_b();

            /* ensure capacity for the new a element */
            ensure_cap(i + 1);

            /* recurrence calculation */
            seq_t val = a_seq[i - 1] + a_seq[i - 2]
                      + b_seq[i - 1] + b_seq[i - 2] + b_seq[i - 3];

            a_seq[i] = val;
            ++a_len;
        }
    }

    return a_seq[(size_t)n];
}
