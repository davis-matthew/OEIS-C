/*
 * A309384  Number of black squares after n moves of a variant of Langton's
 *          ant with turns of 45 degrees.
 * https://oeis.org/A309384
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of black squares after n moves of Langton's ant
 *            variant with 45° turns. The function simulates the ant’s walk
 * Range:     u8 n <= 963, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Directions: 0=E, 1=NE, 2=N, 3=NW, 4=W, 5=SW, 6=S, 7=SE */
static const int32_t DR[8] = { 1, 1, 0,-1,-1,-1, 0, 1};
static const int32_t DC[8] = { 0, 1, 1, 1, 0,-1,-1,-1};

/* Hash table entry */
typedef struct {
    uint64_t key;
    uint8_t  status;          /* 0 = empty, 1 = occupied, 2 = deleted */
} HTEntry;

#define HT_CAP (1 << 17)      /* 131072 – large enough for the expected n */

/* Simple hash function */
static inline uint64_t hash64(uint64_t k) {
    return k * 11400714785074694791ULL;
}

/* Table initialisation */
static void ht_init(HTEntry *ht) {
    for (size_t i = 0; i < HT_CAP; ++i)
        ht[i].status = 0;
}

/* Membership test */
static inline bool ht_has(const HTEntry *ht, uint64_t key) {
    size_t idx = hash64(key) & (HT_CAP - 1);
    for (;;) {
        uint8_t s = ht[idx].status;
        if (s == 0)                 return false;          /* empty */
        if (s == 1 && ht[idx].key == key) return true;     /* found */
        idx = (idx + 1) & (HT_CAP - 1);                    /* probe */
    }
}

/* Insert (assumes key not present) */
static inline void ht_insert(HTEntry *ht, uint64_t key) {
    size_t idx = hash64(key) & (HT_CAP - 1);
    for (;;) {
        uint8_t s = ht[idx].status;
        if (s == 0 || s == 2) {          /* empty or deleted */
            ht[idx].key = key;
            ht[idx].status = 1;
            return;
        }
        idx = (idx + 1) & (HT_CAP - 1);
    }
}

/* Remove key (if present) */
static inline void ht_remove(HTEntry *ht, uint64_t key) {
    size_t idx = hash64(key) & (HT_CAP - 1);
    for (;;) {
        uint8_t s = ht[idx].status;
        if (s == 0) return;                 /* not present */
        if (s == 1 && ht[idx].key == key) {
            ht[idx].status = 2;              /* mark deleted */
            return;
        }
        idx = (idx + 1) & (HT_CAP - 1);
    }
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;

    int32_t x = 0, y = 0;          /* current position */
    int dir = 0;                   /* current direction (0=E) */
    seq_t black = 0;                /* number of black squares */

    /* Allocate hash table */
    HTEntry *ht = (HTEntry *)malloc(HT_CAP * sizeof(HTEntry));
    if (!ht) return (seq_t)0;      /* allocation failure – unlikely */
    ht_init(ht);

    for (idx_t step = 0; step < n; ++step) {
        uint64_t key = ((uint64_t)(int32_t)x << 32) | (uint32_t)(int32_t)y;

        if (ht_has(ht, key)) {                 /* black square */
            ht_remove(ht, key);
            --black;                           /* flip to white */
            dir = (dir + 7) & 7;                /* turn right 45° */
        } else {                                /* white square */
            ht_insert(ht, key);
            ++black;                           /* flip to black */
            dir = (dir + 1) & 7;                /* turn left 45° */
        }

        /* move forward */
        x += DR[dir];
        y += DC[dir];
    }

    free(ht);
    return black;
}
