/*
 * A255774  Tree of upper Wythoff numbers (A001950) generated as the
 *          2-component of the graph described at A095903.
 * https://oeis.org/A255774
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth number encountered in a breadth‑first traversal *
 *            of the lazy‑Fibonacci tree whose nodes are the sums of distinct
 *            Fibonacci * numbers; only nodes that are upper Wythoff numbers
 *            (A001951) are kept
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* --- auxiliary types ---------------------------------------------------- */
typedef struct {
    seq_t val;      /* node value                                            */
    uint32_t k;     /* largest Fibonacci index used in this node             */
} Node;

/* --- constants and static storage ------------------------------------- */
#define MAX_VAL 5000000                /* enough for all n ≤ 10000                */
static unsigned char is_upper_arr[MAX_VAL + 1];   /* 1 if value is an upper Wythoff number */
static unsigned char used_arr[MAX_VAL + 1];      /* helper for Wythoff generation */
static seq_t fib[64];                          /* Fibonacci numbers, indices ≥ 1       */
static size_t fib_cnt;                         /* number of Fibonacci numbers stored  */

/* queue of nodes to process (dynamic array) */
static Node *queue = NULL;
static size_t q_head = 0, q_size = 0, q_cap = 0;

/* array of produced upper Wythoff numbers (up to current request) */
static seq_t *outputs = NULL;
static size_t outputs_size = 0, outputs_cap = 0;

/* flag indicating one‑time initialization */
static bool initialized = false;

/* ------------------------------------------------------------------------ */
/* Utility: push a node onto the queue, resizing if needed */
static void push_queue(seq_t v, uint32_t k)
{
    if (q_size == q_cap) {
        q_cap = q_cap ? q_cap << 1 : 64;
        queue = (Node *)realloc(queue, q_cap * sizeof(Node));
    }
    queue[q_size++] = (Node){v, k};
}

/* Utility: push a result number onto the outputs array, resizing if needed */
static void push_output(seq_t v)
{
    if (outputs_size == outputs_cap) {
        outputs_cap = outputs_cap ? outputs_cap << 1 : 64;
        outputs = (seq_t *)realloc(outputs, outputs_cap * sizeof(seq_t));
    }
    outputs[outputs_size++] = v;
}

/* ------------------------------------------------------------------------ */
/* One‑time initialization procedure */
static void init(void)
{
    if (initialized) return;

    /* --- generate upper Wythoff numbers up to MAX_VAL ------------------- */
    memset(used_arr, 0, sizeof(used_arr));
    memset(is_upper_arr, 0, sizeof(is_upper_arr));
    size_t next_min = 1;
    for (size_t n = 1;; ++n) {
        while (next_min <= MAX_VAL && used_arr[next_min]) ++next_min;
        if (next_min > MAX_VAL) break;
        size_t a = next_min;          /* lower Wythoff number */
        size_t b = a + n;             /* upper Wythoff number */
        used_arr[a] = 1;
        if (b <= MAX_VAL) {
            used_arr[b] = 1;
            is_upper_arr[b] = 1;      /* mark as upper Wythoff */
        }
    }

    /* --- precompute Fibonacci numbers ----------------------------------- */
    fib[0] = 0;
    fib[1] = 1;
    fib[2] = 1;
    fib_cnt = 3;
    /* continue until a Fibonacci number exceeds MAX_VAL + a margin          */
    while (fib[fib_cnt - 1] <= MAX_VAL + 1000 && fib_cnt < 60) {
        fib[fib_cnt] = fib[fib_cnt - 1] + fib[fib_cnt - 2];
        ++fib_cnt;
    }

    /* --- initialize queue with the two starting nodes ------------------- */
    q_cap = 64;
    queue = (Node *)malloc(q_cap * sizeof(Node));
    q_size = 0;
    q_head = 0;
    push_queue((seq_t)1, 2);   /* 1 uses Fibonacci index 2 */
    push_queue((seq_t)2, 3);   /* 2 uses Fibonacci index 3 */

    /* --- outputs array ---------------------------------------------- */
    outputs_cap = 64;
    outputs = (seq_t *)malloc(outputs_cap * sizeof(seq_t));
    outputs_size = 0;

    initialized = true;
}

/* ------------------------------------------------------------------------ */
/* Ensure we have generated at least 'n' upper Wythoff numbers              */
static void ensure_outputs(idx_t n)
{
    init();
    while (outputs_size < n) {
        if (q_head >= q_size) break;     /* safety: queue exhausted unexpectedly */

        Node cur = queue[q_head++];
        seq_t v = cur.val;
        uint32_t k = cur.k;

        if (v <= MAX_VAL && is_upper_arr[(size_t)v]) {
            push_output(v);
        }

        /* generate children if their values stay within the bound        */
        if (k + 1 < fib_cnt) {
            seq_t child1 = v + fib[k + 1];
            if (child1 <= MAX_VAL) push_queue(child1, k + 1);
        }
        if (k + 2 < fib_cnt) {
            seq_t child2 = v + fib[k + 2];
            if (child2 <= MAX_VAL) push_queue(child2, k + 2);
        }
    }
}

/* ------------------------------------------------------------------------ */
/* Public function -------------------------------------------------------- */
seq_t a(idx_t n)
{
    ensure_outputs(n);
    /* assume n >= 1 and outputs_size >= n by the loop above              */
    return outputs[n - 1];
}
