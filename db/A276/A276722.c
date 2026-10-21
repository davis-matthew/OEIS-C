/*
 * A276722  Number of set partitions of [n] such that for each block b the
 *          smallest integer interval containing b has at most six elements.
 * https://oeis.org/A276722
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of set partitions of [n] such that for each block
 *            the * smallest integer interval containing the block has at most
 *            six elements
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 20, u64 n <= 40, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  State: counts of open blocks of age 0..5 (age = current index – block minimum).
 *  Transition: at each new element, ages increase by one; a block with age 5
 *  cannot survive the next increase.  The new element either starts a new block
 *  (age 0) or joins an existing block of age 0..5.  Blocks automatically
 *  close when their age would become 6.  The number of states is 924
 *  (all 6-tuples of non‑negative integers with sum ≤6).                       */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static int init_done = 0;

typedef uint8_t byte;

# define MAX_STATE_NUM 924
# define CODE_BASE 7            /* to encode a 6‑tuple of 0..6 */

/* 6‑tuple of counts for each age */
static byte state_counts[MAX_STATE_NUM][6];
static int inc_id[MAX_STATE_NUM];          /* id after age increment (age += 1, old age 5 discarded) */
static int new_block_id[MAX_STATE_NUM];    /* id after adding a new block (×age 0) */

/* mapping from code to state id */
static int *code_to_id = NULL;
static size_t N_STATES = 0;

/* encode a 6‑tuple into a unique integer (base 7) */
static inline int encode_counts(const byte *cnt)
{
    int code = 0;
    for (int i = 5; i >= 0; --i)            /* pack in base 7, most significant first */
        code = code * CODE_BASE + cnt[i];
    return code;
}

/* generate all states and pre‑compute transitions */
static void generate_states(void)
{
    /* maximum number of distinct 6‑tuples with sum ≤6 */
    N_STATES = 0;
    for (int c0 = 0; c0 <= 6; ++c0)
        for (int c1 = 0; c1 <= 6 - c0; ++c1)
            for (int c2 = 0; c2 <= 6 - c0 - c1; ++c2)
                for (int c3 = 0; c3 <= 6 - c0 - c1 - c2; ++c3)
                    for (int c4 = 0; c4 <= 6 - c0 - c1 - c2 - c3; ++c4)
                        for (int c5 = 0; c5 <= 6 - c0 - c1 - c2 - c3 - c4; ++c5) {
                            byte arr[6] = {
                                (byte)c0,(byte)c1,(byte)c2,
                                (byte)c3,(byte)c4,(byte)c5
                            };
                            memcpy(state_counts[N_STATES], arr, 6);
                            ++N_STATES;
                        }

    /* create code → id table */
    size_t table_size = CODE_BASE * CODE_BASE * CODE_BASE * CODE_BASE *
                        CODE_BASE * CODE_BASE;      /* 7^6 = 117649 */
    code_to_id = (int *)malloc(table_size * sizeof(int));
    for (size_t i = 0; i < table_size; ++i) code_to_id[i] = -1;
    for (size_t sid = 0; sid < N_STATES; ++sid) {
        int code = encode_counts(state_counts[sid]);
        code_to_id[code] = (int)sid;
    }

    /* pre‑compute transitions */
    for (size_t sid = 0; sid < N_STATES; ++sid) {
        byte oldc[6];
        memcpy(oldc, state_counts[sid], 6);

        /* shift ages (age += 1, old age 5 discarded) */
        byte incc[6];
        memset(incc, 0, 6);
        for (int a = 0; a < 5; ++a) incc[a + 1] = oldc[a];
        incc[5] = oldc[4];       /* old age 4 → new age 5 */

        int inc_code = encode_counts(incc);
        inc_id[sid] = code_to_id[inc_code];

        /* start a new block (age 0) */
        incc[0] = 1;
        int new_code = encode_counts(incc);
        new_block_id[sid] = code_to_id[new_code];
    }
}

/* main DP routine */
seq_t a(idx_t n)
{
    if (!init_done) {
        generate_states();
        init_done = 1;
    }

    /* DP buffers */
    static seq_t *cur = NULL, *next = NULL;
    if (!cur) {
        cur  = (seq_t *)calloc(N_STATES, sizeof(seq_t));
        next = (seq_t *)calloc(N_STATES, sizeof(seq_t));
    }

    /* initialise: empty state (all counts zero) */
    memset(cur, 0, N_STATES * sizeof(seq_t));
    cur[0] = (seq_t)1;   /* state id 0 corresponds to all zeros */

    for (idx_t step = 1; step <= n; ++step) {
        memset(next, 0, N_STATES * sizeof(seq_t));

        for (size_t sid = 0; sid < N_STATES; ++sid) {
            seq_t val = cur[sid];
            if (!val) continue;

            /* 1. start a new block */
            next[new_block_id[sid]] += val;

            /* 2. add to an existing block (ages 0..5, but inc_id[0]==0) */
            size_t inc_sid = inc_id[sid];
            byte *inc_cnt = state_counts[inc_sid];
            for (int age = 1; age <= 5; ++age) {
                byte cnt = inc_cnt[age];
                if (cnt) next[inc_sid] += val * (seq_t)cnt;
            }
        }
        /* swap buffers */
        seq_t *tmp = cur;
        cur = next;
        next = tmp;
    }

    /* sum over all states to get the answer */
    seq_t result = 0;
    for (size_t sid = 0; sid < N_STATES; ++sid)
        result += cur[sid];
    return result;
}
