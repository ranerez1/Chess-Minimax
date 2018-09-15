#include <stdio.h>
#include "ChessArrayList.h"
#include <stdlib.h>
#include "ChessParams.h"

SPArrayList* spArrayListCreate(int maxSize){
	 if (maxSize <= 0){
	        return NULL;
	    }
	    SPArrayList *arrayList = (SPArrayList*) malloc(sizeof(SPArrayList));
	    if (arrayList == NULL){
	    	printf("Error: spArrayListCreate has failed");
	        return NULL;
	    }
	    arrayList->elements = (MOVE*) calloc(maxSize, sizeof(MOVE));
	    if (arrayList->elements == NULL){
			printf("Error: spArrayListCreate has failed");
	        free(arrayList);
	        return NULL;
	    }
	    arrayList->actualSize = 0;
	    arrayList->maxSize = maxSize;
	    return arrayList;
}

SPArrayList *spArrayListCopy(SPArrayList* src){
	if (src == NULL){
	        return NULL;
	    }
	    int srcMaxSize = src->maxSize;
	    int srcActualSize = src->actualSize;
	    SPArrayList* copyArrayList = spArrayListCreate(srcMaxSize);
	    if (copyArrayList == NULL){
	    	printf("Error: spArrayListCopy has failed");
	        return NULL;
	    }
	    for (int i = 0; i < srcActualSize; i++){
	        *(copyArrayList->elements + i) = *(src->elements + i);
	    }
	    copyArrayList->actualSize = srcActualSize;
	    return copyArrayList;
}

void spArrayListDestroy(SPArrayList* src){
	if (src != NULL){
	        free(src->elements);
	        free(src);
	    }
}

SP_ARRAY_LIST_MESSAGE spArrayListClear(SPArrayList* src){
	if (src == NULL){
		spArrayListDestroy(src);
	        return SP_ARRAY_LIST_INVALID_ARGUMENT;
	    }

	    src->actualSize = 0;
	    return SP_ARRAY_LIST_SUCCESS;
}

SP_ARRAY_LIST_MESSAGE spArrayListAddAt(SPArrayList* src, MOVE elem, int index){
	if (src == NULL || index < 0 || index > src->maxSize-1){
	        return SP_ARRAY_LIST_INVALID_ARGUMENT;
	    }
	    if (src->actualSize == src->maxSize){
	    	printf("Array full\n");
	        return SP_ARRAY_LIST_FULL;
	    }
		for(int i=(src->actualSize-1);i>=index;--i){
			src->elements[i+1]=src->elements[i];
		}
	    src->elements[index] = elem;
	    (src->actualSize)++;
	    return SP_ARRAY_LIST_SUCCESS;
}

SP_ARRAY_LIST_MESSAGE spArrayListAddFirst(SPArrayList* src, MOVE elem){
	return spArrayListAddAt(src, elem, 0);
}

SP_ARRAY_LIST_MESSAGE spArrayListAddLast(SPArrayList* src, MOVE elem){
	return spArrayListAddAt(src, elem, src->actualSize);
}

SP_ARRAY_LIST_MESSAGE spArrayListRemoveAt(SPArrayList* src, int index){
	 if (src == NULL || index < 0 || index > src->maxSize-1){
	        return SP_ARRAY_LIST_INVALID_ARGUMENT;
	    }
	    if (src->actualSize == 0){
	        return SP_ARRAY_LIST_EMPTY;
	    }
		for(int i=index; i<src->actualSize; i++){
			src->elements[i]=src->elements[i+1];
		}
	    src->actualSize --;
	    return SP_ARRAY_LIST_SUCCESS;
}

SP_ARRAY_LIST_MESSAGE spArrayListRemoveFirst(SPArrayList* src){
	return spArrayListRemoveAt(src, 0);
}

SP_ARRAY_LIST_MESSAGE spArrayListRemoveLast(SPArrayList* src){
	return spArrayListRemoveAt(src, src->actualSize-1);
}

MOVE spArrayListGetAt(SPArrayList* src, int index){
	MOVE mv;
	if (src == NULL || index < 0 || index > src->actualSize -1){
	     mv.dst.x = mv.dst.y = -1;
	     mv.piece = '_';
	     return mv;
	    }
	    return (src->elements[index]);
}

MOVE spArrayListGetFirst(SPArrayList* src){
	return spArrayListGetAt( src, 0);
}

MOVE spArrayListGetLast(SPArrayList* src){
	return spArrayListGetAt( src, src->actualSize -1);
}

int spArrayListMaxCapacity(SPArrayList* src){
	if (src == NULL){
	        return -1;
	    }
	    return src->maxSize;
}

int spArrayListSize(SPArrayList* src){
	if (src == NULL){
	        return -1;
	    }
	    return src->actualSize;
}

bool spArrayListIsFull(SPArrayList* src){
	 if (src == NULL || src->actualSize < src->maxSize){
	        return false;
	    }
	    else{
	        return true;
	    }
}

bool spArrayListIsEmpty(SPArrayList* src){
	 if (src == NULL || src->actualSize > 0){
	        return false;
	    }
	    else{
	        return true;
	    }
}


