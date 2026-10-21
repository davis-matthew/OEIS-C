/*
 * A298002  Solution of the complementary equation a(n) = a(1)*b(n) -
 *          a(0)*b(n-1) + 4*n, where a(0) = 1, a(1) = 2, b(0) = 3, b(1) = 4,
 *          b(2) = 5, and (b(n)) is the increasing sequence of positive
 *          integers not in (a(n)). See Comments.
 * https://oeis.org/A298002
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 48, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=2;
   For n≥2, a(n)=2*b(n)-b(n-1)+4*n,
   where (b(n)) is the increasing sequence of positive integers not in (a(n)). */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

static seq_t *A = NULL;          /* a(n) values           */
static seq_t *B = NULL;          /* b(n) values           */
static uint8_t *visited = NULL;  /* visited[i]==1 ⇔ i∈a() */

static size_t A_cap = 0, B_cap = 0, visited_cap = 0;
static idx_t max_n = (idx_t)-1;   /* highest n for which a(n) is known */

/* ---------- capacity helpers ------------------------------------------- */
static void ensure_a_cap(size_t need)
{
    if (A_cap >= need) return;
    size_t new_cap = A_cap ? A_cap : 1;
    while (new_cap < need) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(A, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    memset(tmp + A_cap, 0, (new_cap - A_cap) * sizeof(seq_t));
    A = tmp;
    A_cap = new_cap;
}

static void ensure_b_cap(size_t need)
{
    if (B_cap >= need) return;
    size_t new_cap = B_cap ? B_cap : 1;
    while (new_cap < need) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(B, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    memset(tmp + B_cap, 0, (new_cap - B_cap) * sizeof(seq_t));
    B = tmp;
    B_cap = new_cap;
}

static void ensure_vis_cap(size_t need)
{
    if (visited_cap > need) return;   /* need index ≤ visited_cap-1 */
    size_t new_cap = visited_cap ? visited_cap : 1;
    while (new_cap <= need) new_cap <<= 1;
    uint8_t *tmp = (uint8_t *)realloc(visited, new_cap * sizeof(uint8_t));
    if (!tmp) abort();
    memset(tmp + visited_cap, 0, (new_cap - visited_cap) * sizeof(uint8_t));
    visited = tmp;
    visited_cap = new_cap;
}

/* ---------- find next number ≥ start that is not in a() ----------------- */
static seq_t find_next_not_visited(idx_t start)
{
    idx_t v = start;
    for (;; ++v) {
        ensure_vis_cap((size_t)v);
        if (!visited[(size_t)v]) return (seq_t)v;
    }
}

/* ---------- initialization ---------------------------------------------- */
static void init_once(void)
{
    ensure_a_cap(2);
    ensure_b_cap(2);
    ensure_vis_cap(2);

    A[0] = (seq_t)1;  /* a(0) */
    A[1] = (seq_t)2;  /* a(1) */
    visited[(size_t)1] = 1;
    visited[(size_t)2] = 1;

    /* first two complementary numbers */
    B[0] = find_next_not_visited(1);                /* 3 */
    B[1] = find_next_not_visited((idx_t)B[0] + 1);   /* 4 */

    max_n = 1;  /* we have a(0) and a(1) computed */
}

/* ------------------------------------------------------------------------ */
/*  seq_t a(idx_t n)  ----------------------------------------------------- */
seq_t a(idx_t n)
{
    static int initialized = 0;
    if (!initialized) { init_once(); initialized = 1; }

    if (n <= max_n) return A[(size_t)n];

    size_t target = (size_t)n;
    ensure_a_cap(target + 1);
    ensure_b_cap(target + 1);

    for (size_t i = (size_t)max_n + 1; i <= target; ++i) {
        /* obtain next b(i) */
        idx_t start = (idx_t)B[i - 1] + 1;
        seq_t bn = find_next_not_visited(start);
        B[i] = bn;

        /* compute a(i) from recurrence */
        seq_t ai = (seq_t)2 * B[i] - B[i - 1] + (seq_t)((idx_t)4 * (idx_t)i);
        A[i] = ai;

        /* mark ai as visited */
        ensure_vis_cap((size_t)ai);
        visited[(size_t)ai] = 1;
    }

    max_n = target;
    return A[(size_t)n];
}
