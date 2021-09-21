# ============================================================================
#  Name	 : Icons_aif_scalable_dc.mk
#  Part of  : RomPatcherPlus
#
#  Description:
# 
# ============================================================================


ifeq (WINS,$(findstring WINS, $(PLATFORM)))
ZDIR=$(EPOCROOT)epoc32\release\$(PLATFORM)\$(CFG)\Z
else
ZDIR=$(EPOCROOT)epoc32\data\z
endif 


# ----------------------------------------------------------------------------
# TODO: Configure these
# ----------------------------------------------------------------------------

TARGETDIR=$(ZDIR)\resource\apps
ICONTARGETFILENAME=$(TARGETDIR)\IconHider_aif_20024264.mif
HEADERDIR=$(EPOCROOT)epoc32\include
HEADERFILENAME=$(HEADERDIR)\IconHider_aif_20024264.mbg

ICONDIR=..\gfx

do_nothing :
	@rem do_nothing

MAKMAKE : do_nothing

BLD : do_nothing

CLEAN : do_nothing

LIB : do_nothing

CLEANLIB : do_nothing

# ----------------------------------------------------------------------------
# TODO: Configure these.
#
# NOTE 1: DO NOT DEFINE MASK FILE NAMES! They are included automatically by
# MifConv if the mask detph is defined.
#
# NOTE 2: Usually, source paths should not be included in the bitmap
# definitions. MifConv searches for the icons in all icon directories in a
# predefined order, which is currently \s60\icons, \s60\bitmaps2.
# The directory \s60\icons is included in the search only if the feature flag
# __SCALABLE_ICONS is defined.
# ----------------------------------------------------------------------------
# NOTE: if you have JUSTINTIME enabled for your S60 3rd FP1 or newer SDK
# and this command crashes, consider adding "/X" to the command line.
# See <http://forum.nokia.com/document/Forum_Nokia_Technical_Library_v1_35/contents/FNTL/Build_process_fails_at_mif_file_creation_in_S60_3rd_Ed_FP1_SDK.htm>
# ----------------------------------------------------------------------------

RESOURCE : $(ICONTARGETFILENAME)

$(ICONTARGETFILENAME): \
		$(ICONDIR)\enable.svg \
		$(ICONDIR)\disable.svg \
		$(ICONDIR)\code.svg \
		$(ICONDIR)\about.svg \
		$(ICONDIR)\register.svg \
		$(ICONDIR)\tab_sett.svg \
		$(ICONDIR)\tab_prot.svg \
		$(ICONDIR)\update.svg \
		$(ICONDIR)\buy.svg \
		$(ICONDIR)\menu_main_icon.svg
	mifconv $(ICONTARGETFILENAME)  \
		 /H$(HEADERFILENAME)  \
		 /c32,8 $(ICONDIR)\menu_main_icon.svg
		
	mifconv $(TARGETDIR)\IconHiderImg_20024264.mif  \
		 /H$(HEADERDIR)\IconHiderImg_20024264.mbg  \
		 /c32,8 $(ICONDIR)\enable.svg  \
		 $(ICONDIR)\disable.svg  \
		 $(ICONDIR)\code.svg  \
		 $(ICONDIR)\about.svg  \
		 $(ICONDIR)\register.svg  \
		 $(ICONDIR)\tab_sett.svg  \
		 $(ICONDIR)\tab_prot.svg  \
		 $(ICONDIR)\update.svg  \
		 $(ICONDIR)\buy.svg
FREEZE : do_nothing

SAVESPACE : do_nothing

RELEASABLES :
	@echo $(ICONTARGETFILENAME)

FINAL : do_nothing

