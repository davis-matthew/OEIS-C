/*
 * A298108  Solution b( ) of the complementary equation a(n) = a(1)*b(n-1) -
 *          a(0)*b(n-2) + 2*n - 3, where a(0) = 1, a(1) = 2, b(0) = 3, b(1) =
 *          4, and (b(n)) is the increasing sequence of positive integers not
 *          in (a(n)). See Comments.
 * https://oeis.org/A298108
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A298108: the complementary sequence b(n) defined by
 * a(n) = a(1)*b(n-1) - a(0)*b(n-2) + 2*n - 3,
 * with a(0)=1, a(1)=2, b(0)=3, b(1)=4, and (b(n)) the increasing
 * sequence of positive integers not in (a(n)).  This implementation
 * builds the two sequences iteratively and keeps a bitmap of numbers
 * that already belong to either sequence.  It works for all provided
 * numeric types (fixed‑width integers or arbitrary‑precision).
 */

#include <stdlib.h>   /* malloc, realloc, abort */
#include <string.h>   /* memset */

/*--- global state -----------------------------------------------------*/
static seq_t *a_seq    = NULL;      /* a sequence (elements of sequence a) */
static seq_t *b_seq    = NULL;      /* b sequence (elements of sequence b) */
static idx_t  a_cap    = 0;          /* capacity of a_seq */
static idx_t  b_cap    = 0;          /* capacity of b_seq */
static unsigned char *used   = NULL;/* bitmap of numbers already used */
static idx_t  used_cap = 0;         /* capacity (in bytes) of used[] */
static idx_t  cur_n    = 0;          /* largest n already computed */
static idx_t  next_cand = 5;         /* candidate for the next b value */

/*--- utilities --------------------------------------------------------*/
static void ensure_used_cap(idx_t limit) {
    /* ensure used[] can index up to 'limit' */
    if (limit < used_cap) return;
    idx_t new_cap = used_cap ? used_cap << 1 : 64;
    while (new_cap <= limit) new_cap <<= 1;
    unsigned char *tmp = (unsigned char *)realloc(used, new_cap);
    if (!tmp) abort();
    /* zero the newly added bytes */
    memset(tmp + used_cap, 0, new_cap - used_cap);
    used = tmp;
    used_cap = new_cap;
}

static void ensure_seq_cap(idx_t limit) {
    /* ensure capacity for indices up to 'limit' in both a_seq and b_seq */
    if (limit <= a_cap && limit <= b_cap) return;
    idx_t new_cap = limit + 16;
    if (new_cap > a_cap) {
        seq_t *tmp = (seq_t *)realloc(a_seq, new_cap * sizeof(*a_seq));
        if (!tmp) abort();
        a_seq = tmp;
        a_cap = new_cap;
    }
    if (new_cap > b_cap) {
        seq_t *tmp = (seq_t *)realloc(b_seq, new_cap * sizeof(*b_seq));
        if (!tmp) abort();
        b_seq = tmp;
        b_cap = new_cap;
    }
}

/*--- initialization ---------------------------------------------------*/
static void init_sequences(void) {
    if (cur_n != 0) return; /* already initialized */
    ensure_seq_cap(3); /* we need space for indices 0..1 at least */
    a_seq[0] = (seq_t)1;
    a_seq[1] = (seq_t)2;
    b_seq[0] = (seq_t)3;
    b_seq[1] = (seq_t)4;
    cur_n = 1;                /* indices 0 and 1 are known */

    ensure_used_cap(4);
    used[1] = used[2] = used[3] = used[4] = 1;
    next_cand = 5;
}

/*--- main compute routine --------------------------------------------*/
static void compute_up_to(idx_t n) {
    init_sequences();
    if (n <= cur_n) return;
    ensure_seq_cap(n + 1);
    for (idx_t i = cur_n + 1; i <= n; ++i) {
        /* a(i) = a(1)*b(i-1) - a(0)*b(i-2) + 2*i - 3 with a(0)=1,a(1)=2 */
        seq_t ai = (seq_t)2 * b_seq[i-1] - b_seq[i-2] + (seq_t)2 * i - (seq_t)3;
        a_seq[i] = ai;
        /* mark a(i) as used */
        ensure_used_cap((idx_t)ai);
        used[(idx_t)ai] = 1;

        /* find next unused integer for b(i) */
        while (next_cand < used_cap && used[next_cand]) ++next_cand;
        if (next_cand >= used_cap) ensure_used_cap(next_cand);
        b_seq[i] = (seq_t)next_cand;
        used[(idx_t)next_cand] = 1;
        ++next_cand;
    }
    cur_n = n;
}

/*--- public interface -------------------------------------------------*/
seq_t a(idx_t n) {
    compute_up_to(n);
    return b_seq[n];
}
