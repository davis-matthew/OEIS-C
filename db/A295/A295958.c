/*
 * A295958  Solution of the complementary equation a(n) = a(n-1) + a(n-2) +
 *          b(n) + 1, where a(0) = 2, a(1) = 3, b(0) = 1, b(1) = 4, b(2) = 5,
 *          and (a(n)) and (b(n)) are increasing complementary sequences.
 * https://oeis.org/A295958
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + b(n) + 1 (n >= 2) Initial values:
 *            a(0)=2, a(1)=3, b(0)=1, b(1)=4, b(2)=5 (a(n)) and (b(n)) are
 *            complementary increasing sequences
 * Range:     u8 n <= 7, u16 n <= 18, u32 n <= 41, u64 n <= 88, u128 n <= 180,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* static state ----------------------------------------------------- */
static seq_t *a_arr = NULL;   /* a[0..len_a-1] */
static size_t len_a = 0;      /* number of a terms computed           */
static size_t cap_a = 0;      /* current allocation capacity          */

static seq_t *b_arr = NULL;   /* b[0..len_b-1] */
static size_t len_b = 0;      /* number of b terms computed           */
static size_t cap_b = 0;      /* current allocation capacity          */

static seq_t candidate = 1;   /* current candidate for the next b     */
static size_t ia = 0;         /* used a index for candidate search    */
static size_t ib = 0;         /* used b index for candidate search    */

/* ----------------------------------------------------------------- */
static void ensure_capacity_a(size_t need)
{
    if (need <= cap_a) return;
    size_t new_cap = cap_a ? cap_a * 2 : 4;
    if (new_cap < need) new_cap = need;
    a_arr = (seq_t *)realloc(a_arr, new_cap * sizeof(seq_t));
    cap_a = new_cap;
}

static void ensure_capacity_b(size_t need)
{
    if (need <= cap_b) return;
    size_t new_cap = cap_b ? cap_b * 2 : 8;
    if (new_cap < need) new_cap = need;
    b_arr = (seq_t *)realloc(b_arr, new_cap * sizeof(seq_t));
    cap_b = new_cap;
}

/* ----------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* --- one‑time initialization -------------------------------- */
    if (!a_arr) {
        ensure_capacity_a(4);
        a_arr[0] = 2;
        a_arr[1] = 3;
        len_a = 2;

        ensure_capacity_b(8);
        b_arr[0] = 1;
        b_arr[1] = 4;
        b_arr[2] = 5;
        len_b = 3;

        candidate = 1;
        ia = 0;
        ib = 0;
    }

    if (n == 0) return 2;
    if (n == 1) return 3;

    /* --- compute up to the requested index ---------------------- */
    while (len_a <= (size_t)n) {
        /* 1. make sure the next b is available ------------------- */
        if (len_b <= len_a) {
            /* find the next unused integer (candidate) */
            while (true) {
                bool used = false;
                if (ia < len_a && a_arr[ia] == candidate) { used = true; ia++; }
                if (ib < len_b && b_arr[ib] == candidate) { used = true; ib++; }
                if (!used) break;
                candidate++;
            }
            ensure_capacity_b(len_b + 1);
            b_arr[len_b] = candidate;
            len_b++;
            candidate++;          /* start next search from the next integer */
        }

        /* 2. compute a[new_index] --------------------------------- */
        seq_t new_a = a_arr[len_a-1] + a_arr[len_a-2] + b_arr[len_a] + 1;
        ensure_capacity_a(len_a + 1);
        a_arr[len_a] = new_a;
        len_a++;
    }

    return a_arr[(size_t)n];
}
